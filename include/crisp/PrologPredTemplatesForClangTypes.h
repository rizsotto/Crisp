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

#pragma once

#include "clang/AST/DeclBase.h"
#include "clang/AST/Type.h"

#include "crisp/PrologPredBaseTemplates.h"


namespace crisp {

  namespace prolog {

    /// Specialization for \c clang::QualType, that is a smart pointer.
    template <>
    struct Retrieve <clang::QualType> {
      typedef clang::QualType argument_type;

      static inline int
      _(term_t ArgumentT, argument_type* A, llvm::StringRef PredName) {
        if ( !PL_get_pointer(ArgumentT, (void **) A))
          return pl_warning(PredName + ": instantiation fault on first arg");
        return TRUE;
      }
    };

    /// Specialization for \c clang::QualType (that is a smart pointer) as \c
    /// ArgumentType.
    template <typename ResultType,
              ResultType (clang::QualType::* Getter)() const>
    struct Get<clang::QualType, ResultType, Getter> {
      typedef clang::QualType argument_type;
      typedef ResultType result_type;
      static inline result_type _(argument_type A) {
        return (A .* Getter)();
      }
    };

    /// Specialization for \c clang::QualType (that is a smart pointer) as \c
    /// ResultType.
    template <typename ArgumentType,
              clang::QualType (ArgumentType::* Getter)() const>
    struct Get<ArgumentType, clang::QualType, Getter> {
      typedef ArgumentType* argument_type;
      typedef clang::QualType result_type;
      static inline result_type _(argument_type A) {
        return (A ->* Getter)();
      }
    };

    /// Specialization for \c clang::QualType (that is a smart pointer) as
    /// both \c ArgumentType and \c ResultType.
    template <clang::QualType (clang::QualType::* Getter)() const>
    struct Get<clang::QualType, clang::QualType, Getter> {
      typedef clang::QualType argument_type;
      typedef clang::QualType result_type;
      static inline result_type _(argument_type A) {
        return (A .* Getter)();
      }
    };

    /// Specialization for \c clang::QualType (that is a smart pointer) as \c
    /// ArgumentType.
    template <bool (clang::QualType::* Predicate)() const>
    struct Check<clang::QualType, Predicate> {
      typedef clang::QualType argument_type;
      static inline foreign_t  _(argument_type Argument) {
        return (Argument .* Predicate)() ? TRUE : FALSE;
      }
    };

    /// Specialization for \c clang::Qualifiers.
    template <bool (clang::Qualifiers::* Predicate)() const>
    struct Check<clang::Qualifiers, Predicate> {
      typedef clang::Qualifiers argument_type;
      static inline foreign_t  _(argument_type Argument) {
        return (Argument .* Predicate)() ? TRUE : FALSE;
      }
    };

    // Specialization for \c clang::QualType, that is a smart pointer. We
    // still need \c clang::QualType::getTypePtr() when \c Type methods are
    // to be applied, as the smart pointer can sometimes point to a
    // structure different to a \c Type.
    template <>
    struct Unify<clang::QualType> {
      typedef clang::QualType result_type;
      static inline foreign_t _(term_t ResultT, const result_type& Result) {
        return PL_unify_pointer(ResultT, Result.getAsOpaquePtr());
      }
    };

    /// Specialization for \c enum \c clang::AccessSpecifier.
    template <>
    struct Unify<enum clang::AccessSpecifier> {
      typedef enum clang::AccessSpecifier result_type;
      static inline foreign_t _(term_t ResultT, const result_type Result) {
        switch (Result) {
        case clang::AS_public:
          return PL_unify_atom_chars(ResultT, "public");
        case clang::AS_protected:
          return PL_unify_atom_chars(ResultT, "protected");
        case clang::AS_private:
          return PL_unify_atom_chars(ResultT, "private");
        case clang::AS_none:
          return PL_unify_atom_chars(ResultT, "none");
        }
      }
    };

    /// Specialization for \c clang::DeclContext. It is necessary because \c
    /// clang::DeclContext objects need to be dynamically casted into a \c
    /// clang::Decl to be useful, as \c clang::DeclContext doesn't extend \c clang::Decl.
    template <>
    struct Unify<const clang::DeclContext*> {
      typedef const clang::DeclContext* result_type;
      static inline foreign_t _(term_t ResultT, const result_type Result) {
        if ( !Result) return FALSE;
        if ( const clang::Decl* D = llvm::dyn_cast<clang::Decl>(Result))
          return PL_unify_pointer(ResultT, (void *) D); // Cast removes const
        else
          return FALSE;
      }
    };

    /// Specialization for \c
    /// clang::DeclContext::specific_decl_iterator. As this class is
    /// in fact a smart pointer, we don't need dynamic memory
    /// allocation in this case.
    template <typename SpecificDecl>
    struct Context < clang::DeclContext::specific_decl_iterator<SpecificDecl> > {
      typedef clang::DeclContext::specific_decl_iterator<SpecificDecl> iterator_type;
      typedef typename iterator_type::value_type context_type;
      static inline context_type newContext(iterator_type I) {
        return *I;
      }
      static inline void deleteContext(context_type C) {
        // Do nothing.
      }
      static inline void context2iter(context_type C, iterator_type& I) {
        clang::DeclContext::decl_iterator DI(C);
        I = clang::DeclContext::specific_decl_iterator<SpecificDecl>(DI);
      }
      static inline void iter2context(iterator_type I, context_type& C) {
        C = *I;
      }
    };

  } // End namespace crisp::prolog

} // End namespace crisp
