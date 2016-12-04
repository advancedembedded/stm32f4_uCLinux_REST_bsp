/*
    This file is part of the WebKit open source project.
    This file has been generated by generate-bindings.pl. DO NOT MODIFY!

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#include "config.h"

#if ENABLE(TOUCH_EVENTS)

#include "JSTouchList.h"

#include "JSTouch.h"
#include "Touch.h"
#include "TouchList.h"
#include <runtime/Error.h>
#include <runtime/JSNumberCell.h>
#include <runtime/PropertyNameArray.h>
#include <wtf/GetPtr.h>

using namespace JSC;

namespace WebCore {

ASSERT_CLASS_FITS_IN_CELL(JSTouchList);

/* Hash table */

static const HashTableValue JSTouchListTableValues[3] =
{
    { "length", DontDelete|ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsTouchListLength), (intptr_t)0 },
    { "constructor", DontEnum|ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsTouchListConstructor), (intptr_t)0 },
    { 0, 0, 0, 0 }
};

static JSC_CONST_HASHTABLE HashTable JSTouchListTable =
#if ENABLE(PERFECT_HASH_SIZE)
    { 15, JSTouchListTableValues, 0 };
#else
    { 5, 3, JSTouchListTableValues, 0 };
#endif

/* Hash table for constructor */

static const HashTableValue JSTouchListConstructorTableValues[1] =
{
    { 0, 0, 0, 0 }
};

static JSC_CONST_HASHTABLE HashTable JSTouchListConstructorTable =
#if ENABLE(PERFECT_HASH_SIZE)
    { 0, JSTouchListConstructorTableValues, 0 };
#else
    { 1, 0, JSTouchListConstructorTableValues, 0 };
#endif

class JSTouchListConstructor : public DOMConstructorObject {
public:
    JSTouchListConstructor(ExecState* exec, JSDOMGlobalObject* globalObject)
        : DOMConstructorObject(JSTouchListConstructor::createStructure(globalObject->objectPrototype()), globalObject)
    {
        putDirect(exec->propertyNames().prototype, JSTouchListPrototype::self(exec, globalObject), None);
    }
    virtual bool getOwnPropertySlot(ExecState*, const Identifier&, PropertySlot&);
    virtual bool getOwnPropertyDescriptor(ExecState*, const Identifier&, PropertyDescriptor&);
    virtual const ClassInfo* classInfo() const { return &s_info; }
    static const ClassInfo s_info;

    static PassRefPtr<Structure> createStructure(JSValue proto) 
    { 
        return Structure::create(proto, TypeInfo(ObjectType, StructureFlags), AnonymousSlotCount); 
    }
    
protected:
    static const unsigned StructureFlags = OverridesGetOwnPropertySlot | ImplementsHasInstance | DOMConstructorObject::StructureFlags;
};

const ClassInfo JSTouchListConstructor::s_info = { "TouchListConstructor", 0, &JSTouchListConstructorTable, 0 };

bool JSTouchListConstructor::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
{
    return getStaticValueSlot<JSTouchListConstructor, DOMObject>(exec, &JSTouchListConstructorTable, this, propertyName, slot);
}

bool JSTouchListConstructor::getOwnPropertyDescriptor(ExecState* exec, const Identifier& propertyName, PropertyDescriptor& descriptor)
{
    return getStaticValueDescriptor<JSTouchListConstructor, DOMObject>(exec, &JSTouchListConstructorTable, this, propertyName, descriptor);
}

/* Hash table for prototype */

static const HashTableValue JSTouchListPrototypeTableValues[2] =
{
    { "item", DontDelete|Function, (intptr_t)static_cast<NativeFunction>(jsTouchListPrototypeFunctionItem), (intptr_t)1 },
    { 0, 0, 0, 0 }
};

static JSC_CONST_HASHTABLE HashTable JSTouchListPrototypeTable =
#if ENABLE(PERFECT_HASH_SIZE)
    { 0, JSTouchListPrototypeTableValues, 0 };
#else
    { 2, 1, JSTouchListPrototypeTableValues, 0 };
#endif

const ClassInfo JSTouchListPrototype::s_info = { "TouchListPrototype", 0, &JSTouchListPrototypeTable, 0 };

JSObject* JSTouchListPrototype::self(ExecState* exec, JSGlobalObject* globalObject)
{
    return getDOMPrototype<JSTouchList>(exec, globalObject);
}

bool JSTouchListPrototype::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
{
    return getStaticFunctionSlot<JSObject>(exec, &JSTouchListPrototypeTable, this, propertyName, slot);
}

bool JSTouchListPrototype::getOwnPropertyDescriptor(ExecState* exec, const Identifier& propertyName, PropertyDescriptor& descriptor)
{
    return getStaticFunctionDescriptor<JSObject>(exec, &JSTouchListPrototypeTable, this, propertyName, descriptor);
}

const ClassInfo JSTouchList::s_info = { "TouchList", 0, &JSTouchListTable, 0 };

JSTouchList::JSTouchList(NonNullPassRefPtr<Structure> structure, JSDOMGlobalObject* globalObject, PassRefPtr<TouchList> impl)
    : DOMObjectWithGlobalPointer(structure, globalObject)
    , m_impl(impl)
{
}

JSTouchList::~JSTouchList()
{
    forgetDOMObject(this, impl());
}

JSObject* JSTouchList::createPrototype(ExecState* exec, JSGlobalObject* globalObject)
{
    return new (exec) JSTouchListPrototype(JSTouchListPrototype::createStructure(globalObject->objectPrototype()));
}

bool JSTouchList::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
{
    const HashEntry* entry = JSTouchListTable.entry(exec, propertyName);
    if (entry) {
        slot.setCustom(this, entry->propertyGetter());
        return true;
    }
    bool ok;
    unsigned index = propertyName.toUInt32(&ok, false);
    if (ok && index < static_cast<TouchList*>(impl())->length()) {
        slot.setCustomIndex(this, index, indexGetter);
        return true;
    }
    return getStaticValueSlot<JSTouchList, Base>(exec, &JSTouchListTable, this, propertyName, slot);
}

bool JSTouchList::getOwnPropertyDescriptor(ExecState* exec, const Identifier& propertyName, PropertyDescriptor& descriptor)
{
    const HashEntry* entry = JSTouchListTable.entry(exec, propertyName);
    if (entry) {
        PropertySlot slot;
        slot.setCustom(this, entry->propertyGetter());
        descriptor.setDescriptor(slot.getValue(exec, propertyName), entry->attributes());
        return true;
    }
    bool ok;
    unsigned index = propertyName.toUInt32(&ok, false);
    if (ok && index < static_cast<TouchList*>(impl())->length()) {
        PropertySlot slot;
        slot.setCustomIndex(this, index, indexGetter);
        descriptor.setDescriptor(slot.getValue(exec, propertyName), DontDelete | ReadOnly);
        return true;
    }
    return getStaticValueDescriptor<JSTouchList, Base>(exec, &JSTouchListTable, this, propertyName, descriptor);
}

bool JSTouchList::getOwnPropertySlot(ExecState* exec, unsigned propertyName, PropertySlot& slot)
{
    if (propertyName < static_cast<TouchList*>(impl())->length()) {
        slot.setCustomIndex(this, propertyName, indexGetter);
        return true;
    }
    return getOwnPropertySlot(exec, Identifier::from(exec, propertyName), slot);
}

JSValue jsTouchListLength(ExecState* exec, JSValue slotBase, const Identifier&)
{
    JSTouchList* castedThis = static_cast<JSTouchList*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    TouchList* imp = static_cast<TouchList*>(castedThis->impl());
    JSValue result = jsNumber(exec, imp->length());
    return result;
}

JSValue jsTouchListConstructor(ExecState* exec, JSValue slotBase, const Identifier&)
{
    JSTouchList* domObject = static_cast<JSTouchList*>(asObject(slotBase));
    return JSTouchList::getConstructor(exec, domObject->globalObject());
}
void JSTouchList::getOwnPropertyNames(ExecState* exec, PropertyNameArray& propertyNames, EnumerationMode mode)
{
    for (unsigned i = 0; i < static_cast<TouchList*>(impl())->length(); ++i)
        propertyNames.add(Identifier::from(exec, i));
     Base::getOwnPropertyNames(exec, propertyNames, mode);
}

JSValue JSTouchList::getConstructor(ExecState* exec, JSGlobalObject* globalObject)
{
    return getDOMConstructor<JSTouchListConstructor>(exec, static_cast<JSDOMGlobalObject*>(globalObject));
}

JSValue JSC_HOST_CALL jsTouchListPrototypeFunctionItem(ExecState* exec, JSObject*, JSValue thisValue, const ArgList& args)
{
    UNUSED_PARAM(args);
    if (!thisValue.inherits(&JSTouchList::s_info))
        return throwError(exec, TypeError);
    JSTouchList* castedThisObj = static_cast<JSTouchList*>(asObject(thisValue));
    TouchList* imp = static_cast<TouchList*>(castedThisObj->impl());
    unsigned index = args.at(0).toInt32(exec);


    JSC::JSValue result = toJS(exec, castedThisObj->globalObject(), WTF::getPtr(imp->item(index)));
    return result;
}


JSValue JSTouchList::indexGetter(ExecState* exec, JSValue slotBase, unsigned index)
{
    JSTouchList* thisObj = static_cast<JSTouchList*>(asObject(slotBase));
    return toJS(exec, thisObj->globalObject(), static_cast<TouchList*>(thisObj->impl())->item(index));
}
JSC::JSValue toJS(JSC::ExecState* exec, JSDOMGlobalObject* globalObject, TouchList* object)
{
    return getDOMObjectWrapper<JSTouchList>(exec, globalObject, object);
}
TouchList* toTouchList(JSC::JSValue value)
{
    return value.inherits(&JSTouchList::s_info) ? static_cast<JSTouchList*>(asObject(value))->impl() : 0;
}

}

#endif // ENABLE(TOUCH_EVENTS)
