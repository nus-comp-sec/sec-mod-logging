#ifndef events_h
#define events_h

#define NUM_EVENT 51

typedef enum EVENT
{
    EVENT_DEFAULT,
    HTMLDocument_createElement,
    HTMLElement_create,
    HTMLElement_setInnerHTML,
    HTMLElement_setOuterHTML,
    HTMLElement_setInnerText,
    HTMLElement_setOuterText,
    DOMWindow_alert,
    DOMWindow_confirm,
    DOMWindow_prompt,
    WebCore_setCookies,
    JSDocument_setLocation,
    JSLocation_setHref,
    JSLocation_setProtocol,
    JSLocation_setHost,
    JSLocation_setHostname,
    JSLocation_setPort,
    JSLocation_setPathname,
    JSLocation_setSearch,
    JSLocation_setHash,
    JSLocation_replace,
    JSLocation_reload,
    JSLocation_assign,
    JSEventListener_handleEvent,
    Element_getAttribute,
    Element_setAttribute,
    Element_focus,
    Element_blur,
    Document_getElementById,
    Document_createElement,
    Document_createDocumentFragment,
    Document_createTextNode,
    Document_setDocumentURI,
    Document_setTitle,
    Document_open,
    Document_close,
    Document_write,
    Document_cookie,
    Document_setCookie,
    Document_setDomain,
    Document_execCommand,
    Document_createAttribute,
    Document_setWindowAttributeEventListener,
    Node_addEventListener,
    Node_appendChild,
    Node_insertBefore,
    Storage_getItem,
    Storage_setItem,
    Storage_removeItem,
    Storage_clear,
    JavaScript_Execute
}EVENT;

static const char *ArrayEvent[] = 
{
    "Default event", 
    "HTMLDocument::createElement",
    "HTMLElement::create",
    "HTMLElement::setInnerHTML",
    "HTMLElement::setOuterHTML",
    "HTMLElement::setInnerText",
    "HTMLElement::setOuterText",
    "DOMWindow::alert",
    "DOMWindow::confirm",
    "DOMWindow::prompt",
    "WebCore::setCookies",
    "JSDocument::setLocation",
    "JSLocation::setHref",
    "JSLocation::setProtocol",
    "JSLocation::setHost",
    "JSLocation::setHostname",
    "JSLocation::setPort",
    "JSLocation::setPathname",
    "JSLocation::setSearch",
    "JSLocation::setHash",
    "JSLocation::replace",
    "JSLocation::reload",
    "JSLocation::assign",
    "JSEventListener::handleEvent",
    "Element::getAttribute",
    "Element::setAttribute",
    "Element::focus",
    "Element::blur",
    "Document::getElementById",
    "Document::createElement",
    "Document::createDocumentFragment",
    "Document::createTextNode",
    "Document::setDocumentURI",
    "Document::setTitle",
    "Document::open",
    "Document::close",
    "Document::write",
    "Document::cookie",
    "Document::setCookie",
    "Document::setDomain",
    "Document::execCommand",
    "Document::createAttribute",
    "Document::setWindowAttributeEventListener",
    "Node::addEventListener",
    "Node::appendChild",
    "Node::insertBefore",
    "Storage::getItem",
    "Storage::setItem",
    "Storage::removeItem",
    "Storage::clear",
    "JavaScript_Execute"
};

#endif
