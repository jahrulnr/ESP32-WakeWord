# Notification System

A simple, FreeRTOS-native notification system for ESP32 applications. Similar to FreeRTOS queues but with string keys instead of handles.

## Overview

This notification system uses the same principles as FreeRTOS APIs:
- Works with `void*` pointers (you handle casting manually)
- Simple, lightweight design
- Thread-safe using FreeRTOS mutexes
- You manage memory ownership

Perfect for inter-task communication in ESP32 projects.

## Features

- **Thread-safe**: Safe concurrent access with mutexes
- **Lightweight**: Simple design, minimal overhead
- **Key-based**: String keys instead of queue handles
- **Manual casting**: You control type casting

## Basic Usage

### Include the Header

```cpp
#include "Notification.h"
```

### Create a Notification Instance

```cpp
Notification* notification = new Notification();
```

### Send Notifications

```cpp
// Send pointer to static data
static int temperature = 25;
bool success = notification->send("temperature", &temperature);

// Send pointer to string
static const char* status = "READY";
success = notification->send("status", (void*)status);

// Send pointer to struct
static struct {
    int id;
    float value;
} sensor = {1, 23.4f};
success = notification->send("sensor", &sensor);

// Send dynamically allocated data
uint8_t* buffer = malloc(100);
// ... fill buffer ...
success = notification->send("buffer", buffer);
// Note: You still own the buffer!
```

### Consume Notifications

```cpp
// Consume and cast manually
void* data = notification->consume("temperature");
if (data) {
    int* temp = (int*)data;  // Manual casting
    printf("Temperature: %d\n", *temp);
}

// Consume string
data = notification->consume("status");
if (data) {
    const char* status = (const char*)data;  // Manual casting
    printf("Status: %s\n", status);
}

// Consume with timeout
data = notification->consume("sensor", pdMS_TO_TICKS(1000));
if (data) {
    struct { int id; float value; }* sensor = data;  // Manual casting
    printf("Sensor ID: %d, Value: %.1f\n", sensor->id, sensor->value);
}

// Consume dynamic data
data = notification->consume("buffer");
if (data) {
    uint8_t* buffer = (uint8_t*)data;  // Manual casting
    // Use buffer...
    free(buffer);  // Free if it was malloc'd
}
```

## API Reference

### Core Methods

#### `bool send(const char* key, void* data)`
Send a notification with a void* pointer.
- `key`: String identifier
- `data`: Pointer to your data
- Returns: true if successful
- **Note**: You manage the memory, just like FreeRTOS

#### `void* consume(const char* key, TickType_t timeout_ticks = pdMS_TO_TICKS(100))`
Consume a notification and get the void* pointer.
- `key`: String identifier
- `timeout_ticks`: Timeout in FreeRTOS ticks
- Returns: void* pointer (cast it yourself) or nullptr
- **Note**: Just like FreeRTOS - you cast the result

### Management Methods

#### `bool has(const char* key)`
Check if a notification exists.

#### `bool remove(const char* key)`
Remove a notification without consuming it.

#### `void clear()`
Remove all notifications.

#### `size_t count()`
Get number of pending notifications.

#### `bool wait(const char* key, TickType_t timeout_ticks = portMAX_DELAY)`
Wait for a notification to arrive (blocking).

## Usage Patterns

### Static Data (No Memory Management)

```cpp
// Producer
static int counter = 0;
void producerTask(void* param) {
    while (1) {
        counter++;
        notification->send("counter", &counter);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

// Consumer  
void consumerTask(void* param) {
    while (1) {
        void* data = notification->consume("counter", portMAX_DELAY);
        if (data) {
            int* count = (int*)data;  // Manual cast
            printf("Counter: %d\n", *count);
        }
    }
}
```

### Dynamic Data (You Manage Memory)

```cpp
// Producer allocates
void producer() {
    char* message = malloc(50);
    strcpy(message, "Hello World");
    notification->send("message", message);
    // You still own 'message' - don't free yet!
}

// Consumer takes ownership
void consumer() {
    void* data = notification->consume("message");
    if (data) {
        char* message = (char*)data;  // Manual cast
        printf("Message: %s\n", message);
        free(message);  // Now free it
    }
}
```

### Event System

```cpp
// Send event notifications
typedef enum {
    EVENT_BUTTON_PRESS,
    EVENT_SENSOR_READY,
    EVENT_WIFI_CONNECTED
} event_type_t;

static event_type_t event = EVENT_BUTTON_PRESS;
notification->send("system_event", &event);

// Handle events
void* data = notification->consume("system_event");
if (data) {
    event_type_t* event = (event_type_t*)data;  // Manual cast
    switch (*event) {
        case EVENT_BUTTON_PRESS:
            // Handle button
            break;
        // ...
    }
}
```

## Comparison with FreeRTOS Queues

| Feature | FreeRTOS Queue | This Notification |
|---------|----------------|-------------------|
| Identifier | QueueHandle_t | const char* key |
| Data Type | void* | void* |
| Type Safety | Manual casting | Manual casting |
| Memory | You manage | You manage |
| Thread Safe | ✅ | ✅ |
| Blocking | ✅ | ✅ |
| Timeout | ✅ | ✅ |

## Why This Approach?

1. **Familiar**: Works exactly like FreeRTOS APIs
2. **Simple**: No complex templates or automatic memory management
3. **Flexible**: You control memory and casting
4. **Native**: Fits perfectly with FreeRTOS patterns
5. **Lightweight**: Minimal overhead

## Thread Safety

Fully thread-safe using FreeRTOS mutexes. Safe to use from:
- Multiple FreeRTOS tasks
- Different CPU cores
- ISR context (with appropriate timeouts)

## Examples

See `NotificationExample.cpp` for comprehensive usage examples showing the FreeRTOS-native style.
