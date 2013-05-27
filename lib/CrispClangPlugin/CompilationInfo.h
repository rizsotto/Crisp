// CompilationInfo.h -------------------------------------------------*- C++ -*-

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

#include "clang/AST/ASTContext.h"
#include "clang/AST/Mangle.h"
#include "clang/AST/PrettyPrinter.h"
#include "clang/Frontend/CompilerInstance.h"


namespace crisp {

  namespace prolog {

    class CompilationInfo {
    public:
      const clang::CompilerInstance& getCompilerInstance() const;

      const clang::LangOptions& getLangOpts() const;

      const clang::PrintingPolicy& getPrintingPolicy() const;

      const clang::SourceManager& getSourceManager() const;

      clang::MangleContext* getMangleContext();

      friend void newCompilationInfo(clang::CompilerInstance &CI);
      friend void deleteCompilationInfo();

    private:
      CompilationInfo(clang::CompilerInstance &CI)
        : CompilerInstance(CI)
        , LangOptions(CI.getLangOpts())
        , PrintingPolicy(LangOptions)
        , SourceManager(CI.getSourceManager())
        , MangleContext(CI.getASTContext().createMangleContext()) {
      }

      ~CompilationInfo();

      const clang::CompilerInstance &CompilerInstance;
      const clang::LangOptions &LangOptions;
      const clang::PrintingPolicy PrintingPolicy;
      const clang::SourceManager &SourceManager;
      clang::MangleContext *MangleContext;
    };

    CompilationInfo* getCompilationInfo();

    void newCompilationInfo(clang::CompilerInstance &CI);

    void deleteCompilationInfo();

    inline const clang::CompilerInstance&
    CompilationInfo::getCompilerInstance() const {
      return CompilerInstance;
    }

    inline const clang::LangOptions& CompilationInfo::getLangOpts() const {
      return LangOptions;
    }

    inline const clang::PrintingPolicy& CompilationInfo::getPrintingPolicy() const {
      return PrintingPolicy;
    }

    inline const clang::SourceManager& CompilationInfo::getSourceManager() const {
      return SourceManager;
    }

    inline clang::MangleContext* CompilationInfo::getMangleContext() {
      return MangleContext;
    }

  } // End namespace crisp::prolog

} // End namespace crisp
