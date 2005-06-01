/*=========================================================================

  Program:   CMake - Cross-Platform Makefile Generator3
  Module:    $RCSfile$
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2005 Kitware, Inc., Insight Consortium.  All rights reserved.
  See Copyright.txt or http://www.cmake.org/HTML/Copyright.html for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef cmGlobalUnixMakefileGenerator3_h
#define cmGlobalUnixMakefileGenerator3_h

#include "cmGlobalGenerator.h"

class cmGeneratedFileStream;
class cmLocalUnixMakefileGenerator3;

/** \class cmGlobalUnixMakefileGenerator3
 * \brief Write a Unix makefiles.
 *
 * cmGlobalUnixMakefileGenerator3 manages UNIX build process for a tree
 
 
 The basic approach of this generator is to produce Makefiles that will all
 be run with the current working directory set to the Home Output
 directory. The one exception to this is the subdirectory Makefiles which are
 created as a convenience and just cd up to the Home Output directory and
 invoke the main Makefiles. 
 
 The make process starts with Makefile. Makefile should only contain the
 targets the user is likely to invoke directly from a make command line. No
 internal targets should be in this file. Makefile2 contains the internal
 targets that are required to make the process work.
 
 Makefile2 in turn will recursively make targets in the correct order. Each
 target has its own directory <target>.dir and its own makefile build.make in
 that directory. Also in that directory is a couple makefiles per source file
 used by the target. Typically these are named source.obj.build.make and
 source.obj.build.depend.make. The source.obj.build.make contains the rules
 for building, cleaning, and computing dependencies for the given source
 file. The build.depend.make contains additional dependencies that were
 computed during dependency scanning. An additional file called
 source.obj.depend is used as a marker to indicate when dependencies must be
 rescanned.

 Rules for custom commands follow the same model as rules for source files.
 
 */

class cmGlobalUnixMakefileGenerator3 : public cmGlobalGenerator
{
public:
  cmGlobalUnixMakefileGenerator3();
  static cmGlobalGenerator* New() { return new cmGlobalUnixMakefileGenerator3; }

  ///! Get the name for the generator.
  virtual const char* GetName() const {
    return cmGlobalUnixMakefileGenerator3::GetActualName();}
  static const char* GetActualName() {return "Unix Makefiles";}

  /** Get the documentation entry for this generator.  */
  virtual void GetDocumentation(cmDocumentationEntry& entry) const;
  
  ///! Create a local generator appropriate to this Global Generator3
  virtual cmLocalGenerator *CreateLocalGenerator();

  /**
   * Try to determine system infomation such as shared library
   * extension, pthreads, byte order etc.  
   */
  virtual void EnableLanguage(std::vector<std::string>const& languages, cmMakefile *);

  /**
   * Generate the all required files for building this project/tree. This
   * basically creates a series of LocalGenerators for each directory and
   * requests that they Generate.  
   */
  virtual void Generate();

protected:
  void WriteMainMakefile();
  void WriteMainMakefile2();
  void WriteMainCMakefile();
  void WriteMainCMakefileLanguageRules(cmGeneratedFileStream& cmakefileStream);
  void WriteAllRules(cmLocalUnixMakefileGenerator3 *lg, 
                     std::ostream& makefileStream);
  void WriteHelpRule(std::ostream& ruleFileStream);
  
  void WriteConvenienceRules(std::ostream& ruleFileStream, 
                             cmLocalUnixMakefileGenerator3 *,
                             std::set<cmStdString> &emitted);
  void WriteConvenienceRules2(std::ostream& ruleFileStream, 
                              cmLocalUnixMakefileGenerator3 *,
                              bool exclude);

  void WriteDirectoryRules(std::ostream& ruleFileStream, 
                           cmLocalUnixMakefileGenerator3 *lg);
  void WriteDirectoryRules2(std::ostream& ruleFileStream, 
                            cmLocalUnixMakefileGenerator3 *lg);

  void AppendGlobalTargetDepends(std::vector<std::string>& depends,
                                 const cmTarget& target);
  void AppendAnyGlobalDepend(std::vector<std::string>& depends, 
                             const char* name);

  // does this generator need a requires step for any of its targets
  bool NeedRequiresStep(cmLocalUnixMakefileGenerator3 *lg);

};

#endif
