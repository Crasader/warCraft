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

#ifndef __XDOBJECT_H__
#define __XDOBJECT_H__


/**
 * @js NA
 */
class XdObject
{
public:
    unsigned int        m_uID;
protected:
    // count of references
    unsigned int        m_uReference;

public:
    XdObject(void);
    /**
     *  @lua NA
     */
    virtual ~XdObject(void);
    
    void release(void);
    void retain(void);
    XdObject* autorelease(void);
    XdObject* copy(void);
    bool isSingleReference(void) const;
    unsigned int retainCount(void) const;
    virtual bool isEqual(const XdObject* pObject);

};


typedef void (XdObject::*XDSEL_SCHEDULE)(float);
typedef void (XdObject::*XDSEL_CallFunc)();
typedef void (XdObject::*XDSEL_CallFuncO)(XdObject*);
typedef void (XdObject::*XDSEL_CallFuncND)(XdObject*, void*);
typedef void (XdObject::*XDSEL_MenuHandler)(XdObject*);
typedef int (XdObject::*XDSEL_Compare)(XdObject*);

#define xdschedule_selector(_SELECTOR) (XDSEL_SCHEDULE)(&_SELECTOR)
#define xdcallfunc_selector(_SELECTOR) (XDSEL_CallFunc)(&_SELECTOR)
#define xdcallfuncN_selector(_SELECTOR) (XDSEL_CallFuncO)(&_SELECTOR)
#define xdcallfuncND_selector(_SELECTOR) (XDSEL_CallFuncND)(&_SELECTOR)
#define xdcallfuncO_selector(_SELECTOR) (XDSEL_CallFuncO)(&_SELECTOR)
#define xdmenu_selector(_SELECTOR) (XDSEL_MenuHandler)(&_SELECTOR)
#define xdevent_selector(_SELECTOR) (XDSEL_EventHandler)(&_SELECTOR)
#define xdcompare_selector(_SELECTOR) (XDSEL_Compare)(&_SELECTOR)

// end of base_nodes group

#endif // __XdObject_H__
