/****************************************************************************
Copyright (c) 2010 cocos2d-x.org

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/


#include "XdObject.h"


XdObject::XdObject(void)
:m_uReference(1) // when the object is created, the reference count of it is 1
,m_uID(0)
{
    static unsigned int uObjectCount = 0;

    m_uID = ++uObjectCount;
}

XdObject::~XdObject(void)
{

}


void XdObject::release(void)
{
    if (m_uReference <= 0) {

        return;
    }
    --m_uReference;

    if (m_uReference == 0)
    {
        delete this;
    }
}

void XdObject::retain(void)
{
    if (m_uReference==0)return;
    ++m_uReference;
}

XdObject* XdObject::autorelease(void)
{
    return this;
}

bool XdObject::isSingleReference(void) const
{
    return m_uReference == 1;
}

unsigned int XdObject::retainCount(void) const
{
    return m_uReference;
}

bool XdObject::isEqual(const XdObject *pObject)
{
    return this == pObject;
}
