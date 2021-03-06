// CrispModulePass.cpp -----------------------------------------------*- C++ -*-

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

/// \file
/// \brief Crisp LLVM Module pass plugin entry point.

#define DEBUG_TYPE "crisp"

#include <string>

#include "llvm/ADT/Statistic.h"
#include "llvm/Analysis/AliasAnalysis.h"
#include "llvm/Module.h"
#include "llvm/Pass.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Target/TargetData.h"

#include "crisp/RunPrologEngine.h"
#include "LLVMCompilationInfo.h"
#include "LLVMPrologPredicateRegistration.h"
#include "LLVMPrologQueries.h"

using namespace llvm;
using namespace crisp::prolog;

STATISTIC(StNumFunctionsMod, "Number of modules analyzed by crisp-mod");

namespace crisp {
  unsigned NumFunctionsMod = 0;

  /// Command line flag to enable prolog interactive session.
  cl::opt<bool> FlagInteractive("crisp-interactive",
                                cl::desc("Enable interactive Prolog session"));

  /// Command line option for Rules file
  cl::opt<std::string>
  RulesFileName("crisp-rules-file",
                cl::desc("Specify Rules filename"),
                cl::value_desc("filename"),
                cl::Required);

  /// \brief Class implementing a module pass to analyze LLVM IR
  /// code for Crisp.
  class CrispModulePass : public ModulePass {
  public:
    static char ID;
    CrispModulePass() : ModulePass(ID) {}
    virtual bool runOnModule(Module& M);
    virtual void getAnalysisUsage(AnalysisUsage& AU) const;
    virtual void releaseMemory();
    // TODO: implement print method.
    // virtual void print(std::ostream &O, const Module *M) const;
  private:
    int Success;                // Prolog Engine status
  };                            // end of struct CrispModulePass

  char CrispModulePass::ID = 0;

  static RegisterPass<CrispModulePass>
  Mod("crisp-mod", "Data extraction module pass for CRISP",
      false,                    // If true, only looks at CFG
      true);                    // If true, analysis Pass

  bool CrispModulePass::runOnModule(Module& M) {
    // Initialization
    DEBUG(dbgs() << "Initializing Crisp Module Pass.\n");
    DEBUG(dbgs() << "Handling LLVM Module.\n");
    plRegisterPredicates();
    Success = plRunEngine("PrologBootForCrispLLVMPass.sh");

    if (Success) {
      Success = plLoadFile(RulesFileName);
      DEBUG(if ( !Success) dbgs() << "Error loading rules file '"
                                  << RulesFileName << "'." << "\n");
    }

    if (Success) {
      // Read clang Prolog facts about the module
      const std::string& ModuleId(M.getModuleIdentifier());
      DEBUG(dbgs() << "LLVM Module name: " << ModuleId << "\n");

      Success = plReadModuleFacts(ModuleId.c_str());

      if (Success) {
        DEBUG(dbgs() << "Processing module: " << M.getModuleIdentifier()
              << "\n");
        (void) plAssertModule(&M);

        // Set some global data to be accessed from Prolog (var
        // CompilationInfo defined in CompilationInfo.h).
        newLLVMCompilationInfo(*this);

        // Main Prolog analysis
        Success = plRunModuleAnalysis();

        // When debugging, open a PROLOG interactive session if
        // FlagInteractive is set
        DEBUG(if (Success && FlagInteractive) Success = plInteractiveSession());

        // Free global data
        deleteLLVMCompilationInfo();
      }
    }

    // Finalization
    StNumFunctionsMod = NumFunctionsMod;

    DEBUG(if (Success) dbgs() << "LLVM Module analyzed.\n";
          else dbgs() << "LLVM Module analysis aborted: "
                      << "Prolog engine failed.\n";);
    (void) plCleanUp(Success ? 0 : 1); // Return value ignored

    return false;                 // M is not modified
  }

  // Analysis pass (it does not modify the program), but has some
  // prerequisites.
  void CrispModulePass::getAnalysisUsage(AnalysisUsage& AU) const {
    AU.setPreservesAll();
    AU.addRequired<AliasAnalysis>();
    AU.addRequired<TargetData>(); // Necessary to get location sizes
  }

  void CrispModulePass::releaseMemory() {
  }

} // End namespace crisp
