/*===================================================================

The Medical Imaging Interaction Toolkit (MITK)

Copyright (c) German Cancer Research Center,
Division of Medical and Biological Informatics.
All rights reserved.

This software is distributed WITHOUT ANY WARRANTY; without
even the implied warranty of MERCHANTABILITY or FITNESS FOR
A PARTICULAR PURPOSE.

See LICENSE.txt or http://www.mitk.org for details.

===================================================================*/

#ifndef mitkPropertyExtension_h
#define mitkPropertyExtension_h

#include <MitkExports.h>

namespace mitk
{
  /** \brief Base class for all property extensions.
    */
  class MITK_CORE_EXPORT PropertyExtension
  {
  public:
    virtual ~PropertyExtension();

  protected:
    PropertyExtension();

  private:
    PropertyExtension(const PropertyExtension& other);
    PropertyExtension& operator=(PropertyExtension other);
  };
}

#endif
