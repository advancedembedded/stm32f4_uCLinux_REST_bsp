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

#if ENABLE(EVENTSOURCE)

#include "JSEventSource.h"

#include "Event.h"
#include "EventListener.h"
#include "EventSource.h"
#include "JSEvent.h"
#include "JSEventListener.h"
#include "KURL.h"
#include "RegisteredEventListener.h"
#include <runtime/Error.h>
#include <runtime/JSNumberCell.h>
#include <runtime/JSString.h>
#include <wtf/GetPtr.h>

using namespace JSC;

namespace WebCore {

ASSERT_CLASS_FITS_IN_CELL(JSEventSource);

/* Hash table */

static const HashTableValue JSEventSourceTableValues[6] =
{
    { "URL", DontDelete|ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsEventSourceURL), (intptr_t)0 },
    { "readyState", DontDelete|ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsEventSourceReadyState), (intptr_t)0 },
    { "onopen", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsEventSourceOnopen), (intptr_t)setJSEventSourceOnopen },
    { "onmessage", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsEventSourceOnmessage), (intptr_t)setJSEventSourceOnmessage },
    { "onerror", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsEventSourceOnerror), (intptr_t)setJSEventSourceOnerror },
    { 0, 0, 0, 0 }
};

static JSC_CONST_HASHTABLE HashTable JSEventSourceTable =
#if ENABLE(PERFECT_HASH_SIZE)
    { 127, JSEventSourceTableValues, 0 };
#else
    { 17, 15, JSEventSourceTableValues, 0 };
#endif

/* Hash table for prototype */

static const HashTableValue JSEventSourcePrototypeTableValues[8] =
{
    { "CONNECTING", DontDelete|ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsEventSourceCONNECTING), (intptr_t)0 },
    { "OPEN", DontDelete|ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsEventSourceOPEN), (intptr_t)0 },
    { "CLOSED", DontDelete|ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsEventSourceCLOSED), (intptr_t)0 },
    { "close", DontDelete|Function, (intptr_t)static_cast<NativeFunction>(jsEventSourcePrototypeFunctionClose), (intptr_t)0 },
    { "addEventListener", DontDelete|Function, (intptr_t)static_cast<NativeFunction>(jsEventSourcePrototypeFunctionAddEventListener), (intptr_t)3 },
    { "removeEventListener", DontDelete|Function, (intptr_t)static_cast<NativeFunction>(jsEventSourcePrototypeFunctionRemoveEventListener), (intptr_t)3 },
    { "dispatchEvent", DontDelete|Function, (intptr_t)static_cast<NativeFunction>(jsEventSourcePrototypeFunctionDispatchEvent), (intptr_t)1 },
    { 0, 0, 0, 0 }
};

static JSC_CONST_HASHTABLE HashTable JSEventSourcePrototypeTable =
#if ENABLE(PERFECT_HASH_SIZE)
    { 127, JSEventSourcePrototypeTableValues, 0 };
#else
    { 17, 15, JSEventSourcePrototypeTableValues, 0 };
#endif

static const HashTable* getJSEventSourcePrototypeTable(ExecState* exec)
{
    return getHashTableForGlobalData(exec->globalData(), &JSEventSourcePrototypeTable);
}
const ClassInfo JSEventSourcePrototype::s_info = { "EventSourcePrototype", 0, 0, getJSEventSourcePrototypeTable };

JSObject* JSEventSourcePrototype::self(ExecState* exec, JSGlobalObject* globalObject)
{
    return getDOMPrototype<JSEventSource>(exec, globalObject);
}

bool JSEventSourcePrototype::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
{
    return getStaticPropertySlot<JSEventSourcePrototype, JSObject>(exec, getJSEventSourcePrototypeTable(exec), this, propertyName, slot);
}

bool JSEventSourcePrototype::getOwnPropertyDescriptor(ExecState* exec, const Identifier& propertyName, PropertyDescriptor& descriptor)
{
    return getStaticPropertyDescriptor<JSEventSourcePrototype, JSObject>(exec, getJSEventSourcePrototypeTable(exec), this, propertyName, descriptor);
}

static const HashTable* getJSEventSourceTable(ExecState* exec)
{
    return getHashTableForGlobalData(exec->globalData(), &JSEventSourceTable);
}
const ClassInfo JSEventSource::s_info = { "EventSource", 0, 0, getJSEventSourceTable };

JSEventSource::JSEventSource(NonNullPassRefPtr<Structure> structure, JSDOMGlobalObject* globalObject, PassRefPtr<EventSource> impl)
    : DOMObjectWithGlobalPointer(structure, globalObject)
    , m_impl(impl)
{
}

JSEventSource::~JSEventSource()
{
    impl()->invalidateJSEventListeners(this);
    forgetDOMObject(this, impl());
}

void JSEventSource::markChildren(MarkStack& markStack)
{
    Base::markChildren(markStack);
    impl()->markJSEventListeners(markStack);
}

JSObject* JSEventSource::createPrototype(ExecState* exec, JSGlobalObject* globalObject)
{
    return new (exec) JSEventSourcePrototype(JSEventSourcePrototype::createStructure(globalObject->objectPrototype()));
}

bool JSEventSource::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
{
    return getStaticValueSlot<JSEventSource, Base>(exec, getJSEventSourceTable(exec), this, propertyName, slot);
}

bool JSEventSource::getOwnPropertyDescriptor(ExecState* exec, const Identifier& propertyName, PropertyDescriptor& descriptor)
{
    return getStaticValueDescriptor<JSEventSource, Base>(exec, getJSEventSourceTable(exec), this, propertyName, descriptor);
}

JSValue jsEventSourceURL(ExecState* exec, JSValue slotBase, const Identifier&)
{
    JSEventSource* castedThis = static_cast<JSEventSource*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    EventSource* imp = static_cast<EventSource*>(castedThis->impl());
    JSValue result = jsString(exec, imp->url());
    return result;
}

JSValue jsEventSourceReadyState(ExecState* exec, JSValue slotBase, const Identifier&)
{
    JSEventSource* castedThis = static_cast<JSEventSource*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    EventSource* imp = static_cast<EventSource*>(castedThis->impl());
    JSValue result = jsNumber(exec, imp->readyState());
    return result;
}

JSValue jsEventSourceOnopen(ExecState* exec, JSValue slotBase, const Identifier&)
{
    JSEventSource* castedThis = static_cast<JSEventSource*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    EventSource* imp = static_cast<EventSource*>(castedThis->impl());
    if (EventListener* listener = imp->onopen()) {
        if (const JSEventListener* jsListener = JSEventListener::cast(listener)) {
            if (JSObject* jsFunction = jsListener->jsFunction(imp->scriptExecutionContext()))
                return jsFunction;
        }
    }
    return jsNull();
}

JSValue jsEventSourceOnmessage(ExecState* exec, JSValue slotBase, const Identifier&)
{
    JSEventSource* castedThis = static_cast<JSEventSource*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    EventSource* imp = static_cast<EventSource*>(castedThis->impl());
    if (EventListener* listener = imp->onmessage()) {
        if (const JSEventListener* jsListener = JSEventListener::cast(listener)) {
            if (JSObject* jsFunction = jsListener->jsFunction(imp->scriptExecutionContext()))
                return jsFunction;
        }
    }
    return jsNull();
}

JSValue jsEventSourceOnerror(ExecState* exec, JSValue slotBase, const Identifier&)
{
    JSEventSource* castedThis = static_cast<JSEventSource*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    EventSource* imp = static_cast<EventSource*>(castedThis->impl());
    if (EventListener* listener = imp->onerror()) {
        if (const JSEventListener* jsListener = JSEventListener::cast(listener)) {
            if (JSObject* jsFunction = jsListener->jsFunction(imp->scriptExecutionContext()))
                return jsFunction;
        }
    }
    return jsNull();
}

void JSEventSource::put(ExecState* exec, const Identifier& propertyName, JSValue value, PutPropertySlot& slot)
{
    lookupPut<JSEventSource, Base>(exec, propertyName, value, getJSEventSourceTable(exec), this, slot);
}

void setJSEventSourceOnopen(ExecState* exec, JSObject* thisObject, JSValue value)
{
    UNUSED_PARAM(exec);
    EventSource* imp = static_cast<EventSource*>(static_cast<JSEventSource*>(thisObject)->impl());
    imp->setOnopen(createJSAttributeEventListener(exec, value, thisObject));
}

void setJSEventSourceOnmessage(ExecState* exec, JSObject* thisObject, JSValue value)
{
    UNUSED_PARAM(exec);
    EventSource* imp = static_cast<EventSource*>(static_cast<JSEventSource*>(thisObject)->impl());
    imp->setOnmessage(createJSAttributeEventListener(exec, value, thisObject));
}

void setJSEventSourceOnerror(ExecState* exec, JSObject* thisObject, JSValue value)
{
    UNUSED_PARAM(exec);
    EventSource* imp = static_cast<EventSource*>(static_cast<JSEventSource*>(thisObject)->impl());
    imp->setOnerror(createJSAttributeEventListener(exec, value, thisObject));
}

JSValue JSC_HOST_CALL jsEventSourcePrototypeFunctionClose(ExecState* exec, JSObject*, JSValue thisValue, const ArgList& args)
{
    UNUSED_PARAM(args);
    if (!thisValue.inherits(&JSEventSource::s_info))
        return throwError(exec, TypeError);
    JSEventSource* castedThisObj = static_cast<JSEventSource*>(asObject(thisValue));
    EventSource* imp = static_cast<EventSource*>(castedThisObj->impl());

    imp->close();
    return jsUndefined();
}

JSValue JSC_HOST_CALL jsEventSourcePrototypeFunctionAddEventListener(ExecState* exec, JSObject*, JSValue thisValue, const ArgList& args)
{
    UNUSED_PARAM(args);
    if (!thisValue.inherits(&JSEventSource::s_info))
        return throwError(exec, TypeError);
    JSEventSource* castedThisObj = static_cast<JSEventSource*>(asObject(thisValue));
    return castedThisObj->addEventListener(exec, args);
}

JSValue JSC_HOST_CALL jsEventSourcePrototypeFunctionRemoveEventListener(ExecState* exec, JSObject*, JSValue thisValue, const ArgList& args)
{
    UNUSED_PARAM(args);
    if (!thisValue.inherits(&JSEventSource::s_info))
        return throwError(exec, TypeError);
    JSEventSource* castedThisObj = static_cast<JSEventSource*>(asObject(thisValue));
    return castedThisObj->removeEventListener(exec, args);
}

JSValue JSC_HOST_CALL jsEventSourcePrototypeFunctionDispatchEvent(ExecState* exec, JSObject*, JSValue thisValue, const ArgList& args)
{
    UNUSED_PARAM(args);
    if (!thisValue.inherits(&JSEventSource::s_info))
        return throwError(exec, TypeError);
    JSEventSource* castedThisObj = static_cast<JSEventSource*>(asObject(thisValue));
    EventSource* imp = static_cast<EventSource*>(castedThisObj->impl());
    ExceptionCode ec = 0;
    Event* evt = toEvent(args.at(0));


    JSC::JSValue result = jsBoolean(imp->dispatchEvent(evt, ec));
    setDOMException(exec, ec);
    return result;
}

// Constant getters

JSValue jsEventSourceCONNECTING(ExecState* exec, JSValue, const Identifier&)
{
    return jsNumber(exec, static_cast<int>(0));
}

JSValue jsEventSourceOPEN(ExecState* exec, JSValue, const Identifier&)
{
    return jsNumber(exec, static_cast<int>(1));
}

JSValue jsEventSourceCLOSED(ExecState* exec, JSValue, const Identifier&)
{
    return jsNumber(exec, static_cast<int>(2));
}

JSC::JSValue toJS(JSC::ExecState* exec, JSDOMGlobalObject* globalObject, EventSource* object)
{
    return getDOMObjectWrapper<JSEventSource>(exec, globalObject, object);
}
EventSource* toEventSource(JSC::JSValue value)
{
    return value.inherits(&JSEventSource::s_info) ? static_cast<JSEventSource*>(asObject(value))->impl() : 0;
}

}

#endif // ENABLE(EVENTSOURCE)
