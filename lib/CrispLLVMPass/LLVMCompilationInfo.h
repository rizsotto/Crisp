// LLVMCompilationInfo.h ---------------------------------------------*- C++ -*-

// Copyright (C) 2011, 2012 Guillem Marpons <gmarpons@babel.ls.fi.upm.es>
//
// This file is part of Crisp.
//
// Crisp is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Crisp is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Crisp.  If not, see <http://www.gnu.org/licenses/>.

#pragma once

/// \file
/// \brief Global data to be used by Prolog predicates implemented in C++.

#include <list>

#include "llvm/Analysis/AliasAnalysis.h"
#include "llvm/Pass.h"

typedef llvm::AliasAnalysis::Location Location;

namespace crisp {

  namespace prolog {

    class LLVMCompilationInfo {
    public:
      const llvm::Pass& getPass() const;
      const llvm::Module& getModule() const;

      std::list<Location>& getLocations();

      friend void newLLVMCompilationInfo(llvm::Pass &P, llvm::Module &M);
      friend void deleteLLVMCompilationInfo();

    private:
      LLVMCompilationInfo(llvm::Pass &P, llvm::Module &M)
        : Pass(P), Module(M) {
      }

      ~LLVMCompilationInfo();

      const llvm::Pass &Pass;
      const llvm::Module &Module;

      // TODO: see if a more efficient data structure than a linked
      // list can be found for Locations. We need that pointers to
      // Locations remain meaningful after new insertions. Related
      // with that is the question if it is convenient to reuse
      // previously computed locations. Different choices:
      // - map
      // - set
      // - rely on some kind of tabling on Prolog side
      std::list<Location> Locations;
    };

    LLVMCompilationInfo* getLLVMCompilationInfo();

    void newLLVMCompilationInfo(llvm::Pass &P, llvm::Module &M);

    void deleteLLVMCompilationInfo();

    inline const llvm::Pass& LLVMCompilationInfo::getPass() const {
      return Pass;
    }

    inline const llvm::Module& LLVMCompilationInfo::getModule() const {
      return Module;
    }

    inline std::list<Location>& LLVMCompilationInfo::getLocations() {
      return Locations;
    }

  } // End namespace crisp::prolog

} // End namespace crisp
