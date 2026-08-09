#pragma once

#include "Basic.hpp"
#include "Text.hpp"
#include <initializer_list>
#include <string>
#include <vector>
#include <iostream>

namespace Wai::Debugging {
    // log entry type
    enum class EntryType {
        Note,
        Warning,
        Error,
        CriticalError,
    };

    // extra data pertaining to the note
    class LogEntrySubdata {
    public:
        std::string name;
        std::string value;

        // constructors
        LogEntrySubdata() {
            name = "[INVLAID]";
            value = "[INVALID]";
        }
        LogEntrySubdata(std::string _name, std::string _value) {
            name = _name;
            value = _value;
        }

        // to string
        std::string ToString() {
            return "\"" + name + "\": \"" + value + "\"";
        }
    };

    // one log entry
    class LogEntry {
    public:
        Wai::Debugging::EntryType type;
        std::string message;
        std::vector<Wai::Debugging::LogEntrySubdata> extras;

        // constructors
        LogEntry() {
            message = "(No message.)";
            type = Wai::Debugging::EntryType::Note;
        }

        // print entry
        std::string ToString(Wai::Tabs tabs) {
            // give correct type string
            std::string entryTypeString;

            // check type
            switch (type) {
            case Wai::Debugging::EntryType::Note:
                entryTypeString = "Note";
                break;
            case Wai::Debugging::EntryType::Warning:
                entryTypeString = "Warning";
                break;
            case Wai::Debugging::EntryType::Error:
                entryTypeString = "Error";
                break;
            case Wai::Debugging::EntryType::CriticalError:
                entryTypeString = "CriticalError";
                break;
            default:
                entryTypeString = "InvalidLogEntryType";
                break;
            }

            // create string output
            // generate tabs
            std::string output = tabs.GenerateTabs();

            // add basics
            output += "{ \"type\": \"" + entryTypeString + "\", \"message\": \"" + message + "\"";

            // add extras
            for (auto extra : extras) {
                output += ", " + extra.ToString();
            }

            // return data after adding closing bracket
            return output + " }";
        }
    };

    // application wide log
    class Log {
        Wai::Tabs tabs;

    public:
        std::vector<Wai::Debugging::LogEntry> entries;
        Wai::Tabs tabManager;
        Wai::Boolean loggingEnabled;
        Wai::Boolean printLogLive;
        Wai::Boolean saveLogToJSONFile;
        Wai::Boolean normalErrorOccured;
        Wai::Boolean criticalErrorOccured;

        // constructors
        Log() {
            tabs = Wai::Tabs(1);
            loggingEnabled = true;
            printLogLive = true;
            saveLogToJSONFile = false;
            normalErrorOccured = false;
            criticalErrorOccured = false;
        }
        Log(Wai::Boolean _loggingEnabled, Wai::Boolean _printLogLive, Wai::Boolean _saveLogToJSONFile, Wai::Boolean _normalErrorOccured, Wai::Boolean _criticalErrorOccured) {
            tabs = Wai::Tabs(1);
            loggingEnabled = _loggingEnabled;
            printLogLive = _printLogLive;
            saveLogToJSONFile = _saveLogToJSONFile;
            normalErrorOccured = _normalErrorOccured;
            criticalErrorOccured = _criticalErrorOccured;
        }

        // add any type of entry to log
        void LogAny(Wai::Debugging::EntryType type, std::string message, Wai::Boolean isNormalError, Wai::Boolean isCriticalError, std::initializer_list<Wai::Debugging::LogEntrySubdata> extras) {
            // check for logging enabled or not
            if (loggingEnabled) {
                // create log entry
                Wai::Debugging::LogEntry entry;
                entry.type = type;
                entry.message = message;
                normalErrorOccured = isNormalError;
                criticalErrorOccured = isCriticalError;
                
                // append extras
                for (auto extra : extras) {
                    entry.extras.push_back(extra);
                }

                // log data
                entries.push_back(entry);

                // if printing log live
                if (printLogLive) {
                    std::string colorValue;

                    // select color based on entry type
                    switch (entry.type) {
                    case Wai::Debugging::EntryType::Note:
                        colorValue = "\033[32m";

                        break;
                    case Wai::Debugging::EntryType::Warning:
                        colorValue = "\033[33m";

                        break;
                    case Wai::Debugging::EntryType::Error:
                        colorValue = "\033[1;31m";

                        break;
                    case Wai::Debugging::EntryType::CriticalError:
                        colorValue = "\033[4;31m";

                        break;
                    default:
                        colorValue = "";

                        break;
                    }

                    // print entry
                    std::cout << colorValue << entry.ToString(tabs.tabDepth) << "\033[0m" << std::endl;
                }
            }
        }

        // add note to log
        void LogNote(std::string message, std::initializer_list<Wai::Debugging::LogEntrySubdata> extras) {
            // log
            LogAny(Wai::Debugging::EntryType::Note, message, false, false, extras);
        }
        void LogNote(std::string message) {
            // log
            LogAny(Wai::Debugging::EntryType::Note, message, false, false, {});
        }

        // add warning to log
        void LogWarning(std::string message, std::initializer_list<Wai::Debugging::LogEntrySubdata> extras) {
            // log
            LogAny(Wai::Debugging::EntryType::Warning, message, false, false, extras);
        }
        void LogWarning(std::string message) {
            // log
            LogAny(Wai::Debugging::EntryType::Warning, message, false, false, {});
        }

        // add error to log
        void LogError(std::string message, std::initializer_list<Wai::Debugging::LogEntrySubdata> extras) {
            // log
            LogAny(Wai::Debugging::EntryType::Error, message, true, false, extras);
        }
        void LogError(std::string message) {
            // log
            LogAny(Wai::Debugging::EntryType::Error, message, true, false, {});
        }

        // add critical error to log
        void LogCriticalError(std::string message, std::initializer_list<Wai::Debugging::LogEntrySubdata> extras) {
            // log
            LogAny(Wai::Debugging::EntryType::CriticalError, message, false, true, extras);
        }
        void LogCriticalError(std::string message) {
            // log
            LogAny(Wai::Debugging::EntryType::CriticalError, message, false, true, {});
        }

        // turn entire log into json string
        std::string ToString() {
            // header
            std::string output = "\"Log\": [\n";

            // append each entry
            for (auto entry : entries) {
                // append entry
                output += entry.ToString(tabs.tabDepth) + "\n";
            }

            // footer
            return output + "]\n";
        }
    };
}
