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
#include "JSLocation.h"

#include "JSLocationCustom.h"
#include "KURL.h"
#include "Location.h"
#include <runtime/Error.h>
#include <runtime/JSString.h>
#include <wtf/GetPtr.h>

using namespace JSC;

namespace WebCore {

ASSERT_CLASS_FITS_IN_CELL(JSLocation);

/* Hash table */

static const HashTableValue JSLocationTableValues[9] =
{
    { "href", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsLocationHref), (intptr_t)setJSLocationHref },
    { "protocol", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsLocationProtocol), (intptr_t)setJSLocationProtocol },
    { "host", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsLocationHost), (intptr_t)setJSLocationHost },
    { "hostname", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsLocationHostname), (intptr_t)setJSLocationHostname },
    { "port", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsLocationPort), (intptr_t)setJSLocationPort },
    { "pathname", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsLocationPathname), (intptr_t)setJSLocationPathname },
    { "search", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsLocationSearch), (intptr_t)setJSLocationSearch },
    { "hash", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsLocationHash), (intptr_t)setJSLocationHash },
    { 0, 0, 0, 0 }
};

static JSC_CONST_HASHTABLE HashTable JSLocationTable =
#if ENABLE(PERFECT_HASH_SIZE)
    { 63, JSLocationTableValues, 0 };
#else
    { 19, 15, JSLocationTableValues, 0 };
#endif

/* Hash table for prototype */

static const HashTableValue JSLocationPrototypeTableValues[5] =
{
    { "assign", DontDelete|Function, (intptr_t)static_cast<NativeFunction>(jsLocationPrototypeFunctionAssign), (intptr_t)1 },
    { "replace", DontDelete|Function, (intptr_t)static_cast<NativeFunction>(jsLocationPrototypeFunctionReplace), (intptr_t)1 },
    { "reload", DontDelete|Function, (intptr_t)static_cast<NativeFunction>(jsLocationPrototypeFunctionReload), (intptr_t)0 },
    { "toString", DontDelete|DontEnum|Function, (intptr_t)static_cast<NativeFunction>(jsLocationPrototypeFunctionToString), (intptr_t)0 },
    { 0, 0, 0, 0 }
};

static JSC_CONST_HASHTABLE HashTable JSLocationPrototypeTable =
#if ENABLE(PERFECT_HASH_SIZE)
    { 15, JSLocationPrototypeTableValues, 0 };
#else
    { 9, 7, JSLocationPrototypeTableValues, 0 };
#endif

const ClassInfo JSLocationPrototype::s_info = { "LocationPrototype", 0, &JSLocationPrototypeTable, 0 };

JSObject* JSLocationPrototype::self(ExecState* exec, JSGlobalObject* globalObject)
{
    return getDOMPrototype<JSLocation>(exec, globalObject);
}

bool JSLocationPrototype::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
{
    return getStaticFunctionSlot<JSObject>(exec, &JSLocationPrototypeTable, this, propertyName, slot);
}

bool JSLocationPrototype::getOwnPropertyDescriptor(ExecState* exec, const Identifier& propertyName, PropertyDescriptor& descriptor)
{
    return getStaticFunctionDescriptor<JSObject>(exec, &JSLocationPrototypeTable, this, propertyName, descriptor);
}

void JSLocationPrototype::put(ExecState* exec, const Identifier& propertyName, JSValue value, PutPropertySlot& slot)
{
    if (putDelegate(exec, propertyName, value, slot))
        return;
    Base::put(exec, propertyName, value, slot);
}

const ClassInfo JSLocation::s_info = { "Location", 0, &JSLocationTable, 0 };

JSLocation::JSLocation(NonNullPassRefPtr<Structure> structure, JSDOMGlobalObject* globalObject, PassRefPtr<Location> impl)
    : DOMObjectWithGlobalPointer(structure, globalObject)
    , m_impl(impl)
{
}

JSLocation::~JSLocation()
{
    forgetDOMObject(this, impl());
}

JSObject* JSLocation::createPrototype(ExecState* exec, JSGlobalObject* globalObject)
{
    return new (exec) JSLocationPrototype(JSLocationPrototype::createStructure(globalObject->objectPrototype()));
}

bool JSLocation::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
{
    if (getOwnPropertySlotDelegate(exec, propertyName, slot))
        return true;
    return getStaticValueSlot<JSLocation, Base>(exec, &JSLocationTable, this, propertyName, slot);
}

bool JSLocation::getOwnPropertyDescriptor(ExecState* exec, const Identifier& propertyName, PropertyDescriptor& descriptor)
{
    if (getOwnPropertyDescriptorDelegate(exec, propertyName, descriptor))
        return true;
    return getStaticValueDescriptor<JSLocation, Base>(exec, &JSLocationTable, this, propertyName, descriptor);
}

JSValue jsLocationHref(ExecState* exec, JSValue slotBase, const Identifier&)
{
    JSLocation* castedThis = static_cast<JSLocation*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    Location* imp = static_cast<Location*>(castedThis->impl());
    JSValue result = jsString(exec, imp->href());
    return result;
}

JSValue jsLocationProtocol(ExecState* exec, JSValue slotBase, const Identifier&)
{
    JSLocation* castedThis = static_cast<JSLocation*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    Location* imp = static_cast<Location*>(castedThis->impl());
    JSValue result = jsString(exec, imp->protocol());
    return result;
}

JSValue jsLocationHost(ExecState* exec, JSValue slotBase, const Identifier&)
{
    JSLocation* castedThis = static_cast<JSLocation*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    Location* imp = static_cast<Location*>(castedThis->impl());
    JSValue result = jsString(exec, imp->host());
    return result;
}

JSValue jsLocationHostname(ExecState* exec, JSValue slotBase, const Identifier&)
{
    JSLocation* castedThis = static_cast<JSLocation*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    Location* imp = static_cast<Location*>(castedThis->impl());
    JSValue result = jsString(exec, imp->hostname());
    return result;
}

JSValue jsLocationPort(ExecState* exec, JSValue slotBase, const Identifier&)
{
    JSLocation* castedThis = static_cast<JSLocation*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    Location* imp = static_cast<Location*>(castedThis->impl());
    JSValue result = jsString(exec, imp->port());
    return result;
}

JSValue jsLocationPathname(ExecState* exec, JSValue slotBase, const Identifier&)
{
    JSLocation* castedThis = static_cast<JSLocation*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    Location* imp = static_cast<Location*>(castedThis->impl());
    JSValue result = jsString(exec, imp->pathname());
    return result;
}

JSValue jsLocationSearch(ExecState* exec, JSValue slotBase, const Identifier&)
{
    JSLocation* castedThis = static_cast<JSLocation*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    Location* imp = static_cast<Location*>(castedThis->impl());
    JSValue result = jsString(exec, imp->search());
    return result;
}

JSValue jsLocationHash(ExecState* exec, JSValue slotBase, const Identifier&)
{
    JSLocation* castedThis = static_cast<JSLocation*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    Location* imp = static_cast<Location*>(castedThis->impl());
    JSValue result = jsString(exec, imp->hash());
    return result;
}

void JSLocation::put(ExecState* exec, const Identifier& propertyName, JSValue value, PutPropertySlot& slot)
{
    if (putDelegate(exec, propertyName, value, slot))
        return;
    lookupPut<JSLocation, Base>(exec, propertyName, value, &JSLocationTable, this, slot);
}

void setJSLocationHref(ExecState* exec, JSObject* thisObject, JSValue value)
{
    static_cast<JSLocation*>(thisObject)->setHref(exec, value);
}

void setJSLocationProtocol(ExecState* exec, JSObject* thisObject, JSValue value)
{
    static_cast<JSLocation*>(thisObject)->setProtocol(exec, value);
}

void setJSLocationHost(ExecState* exec, JSObject* thisObject, JSValue value)
{
    static_cast<JSLocation*>(thisObject)->setHost(exec, value);
}

void setJSLocationHostname(ExecState* exec, JSObject* thisObject, JSValue value)
{
    static_cast<JSLocation*>(thisObject)->setHostname(exec, value);
}

void setJSLocationPort(ExecState* exec, JSObject* thisObject, JSValue value)
{
    static_cast<JSLocation*>(thisObject)->setPort(exec, value);
}

void setJSLocationPathname(ExecState* exec, JSObject* thisObject, JSValue value)
{
    static_cast<JSLocation*>(thisObject)->setPathname(exec, value);
}

void setJSLocationSearch(ExecState* exec, JSObject* thisObject, JSValue value)
{
    static_cast<JSLocation*>(thisObject)->setSearch(exec, value);
}

void setJSLocationHash(ExecState* exec, JSObject* thisObject, JSValue value)
{
    static_cast<JSLocation*>(thisObject)->setHash(exec, value);
}

JSValue JSC_HOST_CALL jsLocationPrototypeFunctionAssign(ExecState* exec, JSObject*, JSValue thisValue, const ArgList& args)
{
    UNUSED_PARAM(args);
    if (!thisValue.inherits(&JSLocation::s_info))
        return throwError(exec, TypeError);
    JSLocation* castedThisObj = static_cast<JSLocation*>(asObject(thisValue));
    return castedThisObj->assign(exec, args);
}

JSValue JSC_HOST_CALL jsLocationPrototypeFunctionReplace(ExecState* exec, JSObject*, JSValue thisValue, const ArgList& args)
{
    UNUSED_PARAM(args);
    if (!thisValue.inherits(&JSLocation::s_info))
        return throwError(exec, TypeError);
    JSLocation* castedThisObj = static_cast<JSLocation*>(asObject(thisValue));
    return castedThisObj->replace(exec, args);
}

JSValue JSC_HOST_CALL jsLocationPrototypeFunctionReload(ExecState* exec, JSObject*, JSValue thisValue, const ArgList& args)
{
    UNUSED_PARAM(args);
    if (!thisValue.inherits(&JSLocation::s_info))
        return throwError(exec, TypeError);
    JSLocation* castedThisObj = static_cast<JSLocation*>(asObject(thisValue));
    return castedThisObj->reload(exec, args);
}

JSValue JSC_HOST_CALL jsLocationPrototypeFunctionToString(ExecState* exec, JSObject*, JSValue thisValue, const ArgList& args)
{
    UNUSED_PARAM(args);
    if (!thisValue.inherits(&JSLocation::s_info))
        return throwError(exec, TypeError);
    JSLocation* castedThisObj = static_cast<JSLocation*>(asObject(thisValue));
    return castedThisObj->toString(exec, args);
}

JSC::JSValue toJS(JSC::ExecState* exec, JSDOMGlobalObject* globalObject, Location* object)
{
    return getDOMObjectWrapper<JSLocation>(exec, globalObject, object);
}
Location* toLocation(JSC::JSValue value)
{
    return value.inherits(&JSLocation::s_info) ? static_cast<JSLocation*>(asObject(value))->impl() : 0;
}

}
