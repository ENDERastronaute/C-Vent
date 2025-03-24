
#pragma once

template<typename T>
Event<T>::Event() : m_handlers(new std::deque<Handler>()) {};

template<typename T>
Event<T>::~Event()
{
    delete m_handlers;
};

template<typename T>
void Event<T>::emit(T data)
{
    for (const auto& handler : *m_handlers)
    {
        handler.call(data);
    }
}

template<typename T>
void Event<T>::connect(void (*func)(T)) const
{
    Handler handler;
    handler.caller = [](void* context, T data)
    {
        auto f = reinterpret_cast<void(*)(T)>(context);
        f(data);
    };
    handler.context = reinterpret_cast<void*>(func);
    handler.type = HandlerType::FUNCTION;
    m_handlers->push_back(handler);
}

template<typename T>
template<typename I>
void Event<T>::connect(I* instance, void (I::*method)(T))
{
    auto ctx = new MethodContext<I>{instance, method};

    Handler handler;
    handler.caller = [](void* context, T data)
    {
        auto caller_ctx = static_cast<MethodContext<I>*>(context);
        (caller_ctx->instance->*(caller_ctx->method))(data);
    };
    handler.context = ctx;
    handler.type = HandlerType::METHOD;
    m_handlers->push_back(handler);
}

template<typename T>
void Event<T>::disconnect(void (*func)(T))
{
    auto it = std::find_if(m_handlers->begin(), m_handlers->end(),
    [func](const Handler& handler) {
        return handler.type == HandlerType::FUNCTION &&
            handler.context == reinterpret_cast<void*>(func);
    });

    if (it != m_handlers->end()) {
        m_handlers->erase(it);
    }
}

template<typename T>
template<typename I>
void Event<T>::disconnect(I *instance, void (I::*method)(T))
{
    auto it = std::find_if(m_handlers->begin(), m_handlers->end(),
    [instance, method](const Handler& handler) {
        if (handler.type != HandlerType::METHOD)
        {
            return false;
        }
        auto ctx = static_cast<MethodContext<I>*>(handler.context);
        return ctx->instance == instance && ctx->method == method;
    });

    if (it != m_handlers->end())
    {
        // Libération de la mémoire du context
        delete static_cast<MethodContext<I>*>(it->context);
        m_handlers->erase(it);
    }
}

template<typename T>
size_t Event<T>::handlers() const
{
    return m_handlers->size();
}

template<typename T>
void Event<T>::Handler::call(T data) const
{
    caller(context, data);
}

template<typename T>
bool Event<T>::Handler::operator==(void (*func)(T))
{
    return caller == reinterpret_cast<void(*)(void*, T)>(func);
}
