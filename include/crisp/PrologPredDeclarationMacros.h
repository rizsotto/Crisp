/* PrologPredDeclarationMacros.h ------------------------------------*- C -*- */

/* Copyright (C) 2011, 2012 Guillem Marpons <gmarpons@babel.ls.fi.upm.es>

   This file is part of Crisp.

   Crisp is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   Crisp is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Crisp.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

/** \file \brief Preprocessor macro definitions to \e declare
 *  functions that implement in C++ Prolog predicates with
 *  functionality automatically grasped from LLVM or Clang sources.
 */

#undef pl_get_one

#define pl_get_one(NAME, ARGTYPE, RESTYPE, CXXNAME)                     \
  foreign_t pl_##ARGTYPE##_##NAME(term_t ArgumentT, term_t ResultT);

#undef pl_check_property

#define pl_check_property(VERB, NAME, ARGTYPE, CXXNAME)         \
  foreign_t pl_##ARGTYPE##_##VERB##_##NAME(term_t ArgumentT);

#undef pl_get_many

#define pl_get_many(NAME, ARGTYPE, ITERTYPE, ITERBEGIN, ITEREND)        \
  foreign_t pl_##ARGTYPE##_##NAME(term_t ArgumentT, term_t ResultT,     \
                                  control_t Handle);
