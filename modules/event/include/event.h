//
// Created by ender on 23.03.25.
//

#ifndef EVENT_H
#define EVENT_H

#include <deque>
#include <functional>

/**
 * @class Event
 * @brief Manages event handlers and emission.
 *
 * Connects global functions or member methods, and triggers the event with data.
 *
 * @tparam T The type of data passed when the event is triggered.
 */
template<typename T>
class Event
{
public:
    explicit Event();

    ~Event();

    /**
     * @brief Triggers the event and calls all connected handlers.
     * @param data Data passed to handlers.
     */
    void emit(T data);

    /**
     * @brief Connects a global function to the event.
     * @param handler The global function to connect.
     */
    void connect(void(*func)(T)) const;


    /**
     * @brief Connects a member method to the event.
     * @tparam I The instance type of the object.
     * @param instance The object instance.
     * @param method The member method to connect.
     */
    template<typename I>
    void connect(I* instance, void (I::*method)(T));

    /**
     * @brief Disconnects a global function from the event.
     * @param handler The global function to disconnect.
     */
    void disconnect(void(*func)(T));

    /**
     * @brief Disconnects a member method from the event.
     * @tparam I The instance type of the object
     * @param instance The object instance.
     * @param method The member method to disconnect.
     */
    template<typename I>
    void disconnect(I* instance, void (I::*method)(T));

    /**
     * @brief Returns the number of handlers connected to the event.
     * @return The count of connected handlers.
     */
    [[nodiscard]] size_t handlers() const;

private:
    enum class HandlerType
    {
        FUNCTION,
        METHOD
    };

    template<typename I>
    struct MethodContext
    {
        I* instance;
        void (I::*method)(T);
    };

    struct Handler
    {
        HandlerType type;
        void* context;
        void (*caller)(void* context, T data);

        void call(T data) const;

        bool operator==(void (*func)(T));
    };

    std::deque<Handler> *m_handlers;
};

#include "event.ipp"

#endif //EVENT_H