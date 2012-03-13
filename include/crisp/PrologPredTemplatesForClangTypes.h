// include/crisp/PrologPredTemplatesForClangTypes.h ------------------*- C++ -*-

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

#ifndef PROLOGPREDTEMPLATESFORCLANGTYPES_H
#define PROLOGPREDTEMPLATESFORCLANGTYPES_H

#include "clang/AST/DeclBase.h"
#include "clang/AST/Type.h"

#include "crisp/PrologPredBaseTemplates.h"

using namespace llvm;
using namespace clang;

namespace crisp {

  namespace prolog {

    /// Specialization for \c QualType, that is a smart pointer.
    template <>
    struct Retrieve <QualType> {
      typedef QualType argument_type;

      static inline int
      _(term_t ArgumentT, argument_type* A, StringRef PredName) {
        if ( !PL_get_pointer(ArgumentT, (void **) A))
          return pl_warning(PredName + ": instantiation fault on first arg");
        return TRUE;
      }
    };

    /// Specialization for \c QualType (that is a smart pointer) as \c
    /// ArgumentType.
    template <typename ResultType,
              ResultType (QualType::* Getter)() const>
    struct Get<QualType, ResultType, Getter> {
      typedef QualType argument_type;
      typedef ResultType result_type;
      static inline result_type _(argument_type A) {
        return (A .* Getter)();
      }
    };

    /// Specialization for \c QualType, that is a smart pointer.
    template <>
    struct Unify<QualType> {
      typedef QualType result_type;
      static inline foreign_t _(term_t ResultT, const result_type& Result) {
        return PL_unify_pointer(ResultT, Result.getAsOpaquePtr());
      }
    };

    /// Specialization for \c DeclContext. It is necessary because \c
    /// DeclContext objects need to be dynamically casted into a \c
    /// Decl to be useful, as \c DeclContext doesn't extend \c Decl.
    template <>
    struct Unify<const DeclContext*> {
      typedef const DeclContext* result_type;
      static inline foreign_t _(term_t ResultT, const result_type Result) {
        if ( !Result) return FALSE;
        if ( const Decl* D = dyn_cast<Decl>(Result))
          return PL_unify_pointer(ResultT, (void *) D); // Cast removes const
        else
          return FALSE;
      }
    };

  } // End namespace crisp::prolog

} // End namespace crisp

#endif
