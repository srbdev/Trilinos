// @HEADER
// ***********************************************************************
//
//          Tpetra: Templated Linear Algebra Services Package
//                 Copyright (2008) Sandia Corporation
//
// Under the terms of Contract DE-AC04-94AL85000 with Sandia Corporation,
// the U.S. Government retains certain rights in this software.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
// 1. Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright
// notice, this list of conditions and the following disclaimer in the
// documentation and/or other materials provided with the distribution.
//
// 3. Neither the name of the Corporation nor the names of the
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY SANDIA CORPORATION "AS IS" AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
// PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL SANDIA CORPORATION OR THE
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// Questions? Contact Michael A. Heroux (maherou@sandia.gov)
//
// ************************************************************************
// @HEADER

#include "Tpetra_Details_initializeKokkos.hpp"
#include "Teuchos_GlobalMPISession.hpp"
#include "Kokkos_Core.hpp"
#include "Tpetra_Details_checkLaunchBlocking.hpp"
#include <cstdlib> // std::atexit
#include <string>
#include <vector>

namespace Tpetra {
namespace Details {

void finalizeKokkosIfNeeded() {
  if(!Kokkos::is_finalized()) {
    Kokkos::finalize();
  }
}
  
void
initializeKokkos ()
{
  if (! Kokkos::is_initialized ()) {
    std::vector<std::string> args = Teuchos::GlobalMPISession::getArgv ();
    int narg = static_cast<int> (args.size ()); // must be nonconst

    std::vector<char*> args_c;
    std::vector<std::unique_ptr<char[]>> args_;
    for (auto const& x : args) {
      args_.emplace_back(new char[x.size() + 1]);
      char* ptr = args_.back().get();
      strcpy(ptr, x.c_str());
      args_c.push_back(ptr);
    }
    args_c.push_back(nullptr);

    Kokkos::initialize (narg, narg == 0 ? nullptr : args_c.data ());
    checkOldCudaLaunchBlocking();

    std::atexit (finalizeKokkosIfNeeded);
  }
}

} // namespace Details
} // namespace Tpetra

