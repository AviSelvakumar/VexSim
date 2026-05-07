// Stub implementation of the LemLog library for the VEX simulator.
// The real LemLog is compiled for ARM (VEX V5) and cannot be linked on Windows.
// This stub routes all log output to stdout so you can see LemLib debug messages.

#include "LemLog/logger/Sink.hpp"
#include <cstdio>
#include <list>
#include <string>

namespace logger {

// ── Global sink registry ──────────────────────────────────────────────────────

static std::list<Sink*> g_sinks;

void log(Level level, const std::string& topic, const std::string& message) {
    const char* lvl = "INFO";
    if (level == Level::DEBUG) lvl = "DEBUG";
    else if (level == Level::WARN)  lvl = "WARN";
    else if (level == Level::ERROR) lvl = "ERROR";
    printf("[%s] [%s] %s\n", lvl, topic.c_str(), message.c_str());
}

// ── Sink base class ───────────────────────────────────────────────────────────

Sink::Sink(std::string name) : m_name(std::move(name)) {
    g_sinks.push_back(this);
}

Sink::~Sink() {
    g_sinks.remove(this);
}

void Sink::addToAllowList(const std::string&)    {}
void Sink::removeFromAllowList(const std::string&) {}
void Sink::addToBlockedList(const std::string&)  {}
void Sink::removeFromBlockedList(const std::string&) {}
void Sink::setLoggingLevel(Level level)           { m_minLevel = level; }

const std::string& Sink::getName() const& { return m_name; }

SinkStatus Sink::send(Level level, const std::string& topic,
                      const std::string& message) {
    if (level < m_minLevel) return SinkStatus::OK;
    return write(level, topic, message);
}

} // namespace logger
