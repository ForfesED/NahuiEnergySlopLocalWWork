// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.

#pragma once
#include <EASTL/string.h>

#include "nau/dag_ioSys/dag_chainedMemIo.h"
#include "nau/input_system.h"
#include "nau/io/file_system.h"
#include "nau/io/stream.h"
#include "nau/service/service_provider.h"

namespace nau::input
{
    eastl::string toString(nau::DataBlock& block)
    {
        nau::iosys::MemorySaveCB save;
        block.saveToTextStream(save);
        auto* mem = save.getMem();
        char buffer[8192] = {};
        unsigned idx = 0;
        while (mem != nullptr)
        {
            for (unsigned i = 0; (i < mem->used) && idx < sizeof(buffer) - 1; ++i, ++idx)
            {
                buffer[idx] = mem->data[i];
            }
            mem = mem->next;
        }
        buffer[idx] = '\0';
        return buffer;
    }

    eastl::string testSerializeKey()
    {
        using namespace nau;
        IInputSystem& insys = getServiceProvider().get<IInputSystem>();
        auto action = insys.addAction("KeyPressedX", IInputAction::Type::Trigger, insys.createSignal("pressed", "keyboard", [&insys](IInputSignal* signal)
        {
            signal->Properties().set("key", eastl::string("x"));
        }),
                                      [](IInputSignal*)
        {
        });
        DataBlock block;
        action->serialize(&block);
        return toString(block);
    }

    eastl::string testSerializeDelayPress()
    {
        using namespace nau;
        IInputSystem& insys = getServiceProvider().get<IInputSystem>();
        auto action = insys.addAction("KeyPressedDelay", IInputAction::Type::Trigger, insys.createSignal("delay", "gate", [&insys](IInputSignal* signal)
        {
            signal->Properties().set("delay", 1.1f);
            signal->addInput(insys.createSignal("pressed", "keyboard", [&insys](IInputSignal* signal)
            {
                signal->Properties().set("key", eastl::string("1"));
            }));
        }),
                                      [](IInputSignal*)
        {
        });
        DataBlock block;
        action->serialize(&block);
        return toString(block);
    }

    eastl::string testSerializeMultiplePress()
    {
        using namespace nau;
        IInputSystem& insys = getServiceProvider().get<IInputSystem>();
        auto* signalMulty = insys.createSignal("multiple", "gate", [&insys](IInputSignal* signal)
        {
            signal->Properties().set("delay", 1.f);
            signal->Properties().set("num", 2);
        });
        signalMulty->addInput(insys.createSignal("pressed", "keyboard", [&insys](IInputSignal* signal)
        {
            signal->Properties().set("key", eastl::string("1"));
        }));
        auto action = insys.addAction("KeyPressedDoubleClick", IInputAction::Type::Trigger, signalMulty, [](IInputSignal*)
        {
        });
        DataBlock block;
        action->serialize(&block);
        return toString(block);
    }

    eastl::string testSerializeMouse()
    {
        using namespace nau;
        IInputSystem& insys = getServiceProvider().get<IInputSystem>();
        IInputSignal* signal = insys.createSignal("move", "mouse", [&insys](IInputSignal* signal)
        {
            signal->Properties().set("axis_x", 0);
            signal->Properties().set("axis_y", 1);
        });
        auto action = insys.addAction("ActionMouse", IInputAction::Type::Continuous, signal, [](IInputSignal*)
        {
        });
        DataBlock block;
        action->serialize(&block);
        return toString(block);
    }

    eastl::string testSerializeOr()
    {
        using namespace nau;
        IInputSystem& insys = getServiceProvider().get<IInputSystem>();

        IInputSignal* signalOr = insys.createSignal("or", "gate", [&insys](IInputSignal* signal)
        {
            signal->addInput(insys.createSignal("pressed", "keyboard", [&insys](IInputSignal* signal)
            {
                signal->Properties().set("key", eastl::string("q"));
            }));
            signal->addInput(insys.createSignal("pressed", "keyboard", [&insys](IInputSignal* signal)
            {
                signal->Properties().set("key", eastl::string("w"));
            }));
        });
        auto action = insys.addAction("KeyPressedQW", IInputAction::Type::Trigger, signalOr, [](IInputSignal*)
        {
        });
        DataBlock block;
        action->serialize(&block);
        return toString(block);
    }

    eastl::string testSerializeKeyAxis()
    {
        using namespace nau;
        IInputSystem& insys = getServiceProvider().get<IInputSystem>();
        auto action = insys.addAction("KeyToAxisR", IInputAction::Type::Continuous, insys.createSignal("key_axis", "keyboard", [&insys](IInputSignal* signal)
        {
            //"0,r,1"
            signal->Properties().set("key", eastl::string("r"));
            signal->Properties().set("axis", 0);
            signal->Properties().set("coeff", 1.f);
        }),
                                      [](IInputSignal*)
        {
        });
        DataBlock block;
        action->serialize(&block);
        return toString(block);
    }

    eastl::string testSerializeWASD()
    {
        using namespace nau;
        IInputSystem& insys = getServiceProvider().get<IInputSystem>();
        // Action name,
        // Action type,
        // Signal
        auto action = insys.addAction("WASD_Mouse", IInputAction::Type::Continuous,
                                      insys.createSignal("or", "gate", [&insys](IInputSignal* signalWasd)
        {
            signalWasd->addInput(insys.createSignal("or", "gate", [&insys](IInputSignal* signalOr)
            {
                signalOr->addInput(insys.createSignal("key_axis", "keyboard", [](IInputSignal* signal)
                {
                    signal->Properties().set("key", eastl::string("w"));
                    signal->Properties().set("axis", 1);
                    signal->Properties().set("coeff", 1.f);
                }));
                signalOr->addInput(insys.createSignal("key_axis", "keyboard", [](IInputSignal* signal)
                {
                    signal->Properties().set("key", eastl::string("s"));
                    signal->Properties().set("axis", 1);
                    signal->Properties().set("coeff", -1.f);
                }));
                signalOr->addInput(insys.createSignal("key_axis", "keyboard", [](IInputSignal* signal)
                {
                    signal->Properties().set("key", eastl::string("a"));
                    signal->Properties().set("axis", 0);
                    signal->Properties().set("coeff", -1.f);
                }));
                signalOr->addInput(insys.createSignal("key_axis", "keyboard", [](IInputSignal* signal)
                {
                    signal->Properties().set("key", eastl::string("d"));
                    signal->Properties().set("axis", 0);
                    signal->Properties().set("coeff", 1.f);
                }));
            }));
            signalWasd->addInput(insys.createSignal("scale", "mouse", [&insys](IInputSignal* signalScale)
            {
                signalScale->Properties().set("scale", 10.f);
                signalScale->addInput(insys.createSignal("move_relative", "mouse", [](IInputSignal* signal)
                {
                    signal->Properties().set("axis_x", 0);
                    signal->Properties().set("axis_y", 1);
                }));
            }));
        }),
                                      [](IInputSignal* onAction)
        {
        });
        DataBlock block;
        action->serialize(&block);
        return toString(block);
    }
    void testDeserialize(const eastl::string& data, bool save)
    {
        using namespace nau;
        auto& insys = getServiceProvider().get<IInputSystem>();
        auto action = insys.addAction(data, [](IInputSignal*)
        {
        });
        DataBlock block;
        action->serialize(&block);
        auto testData = toString(block);
        if (data.compare(testData) != 0)
        {
            std::cout << "Serialization/Deserialization test failed! " << std::endl;
            std::cout << "Original    :" << data.c_str() << std::endl;
            std::cout << "Reserialized:" << testData.c_str() << std::endl;
            NAU_FAILURE();
        }
        if (save)
        {
            eastl::string file("c:\\temp\\");
            file += action->getName();
            file += ".blk";
            insys.saveAction(action, file);
        }
        bool res = insys.removeAction(eastl::move(action));
        NAU_ASSERT(res);
    }

    void dumpDevices()
    {
        using namespace nau;
        auto& insys = getServiceProvider().get<IInputSystem>();
        auto devices = insys.getDevices();
        for (auto& device : devices)
        {
            std::cout << "Device " << device->getName().c_str() << std::endl;
            {
                unsigned num = device->getKeysNum();
                for (unsigned i = 0; i < num; ++i)
                {
                    auto state = device->getKeyState(i);
                    auto name = device->getKeyName(i);
                    std::cout << "  key " << i << " " << name.c_str() << std::endl;
                }
            }
            {
                unsigned num = device->getAxisNum();
                for (unsigned i = 0; i < num; ++i)
                {
                    auto state = device->getAxisState(i);
                    auto name = device->getAxisName(i);
                    std::cout << "  axis " << i << " " << name.c_str() << std::endl;
                }
            }
        }
    }

    void testFullCycle(bool save)
    {
        using namespace nau;
        {
            auto data = testSerializeKey();
            std::cout << data.c_str() << std::endl;
            testDeserialize(data, save);
        }
        {
            auto data = testSerializeMouse();
            std::cout << data.c_str() << std::endl;
            testDeserialize(data, save);
        }

        {
            auto data = testSerializeOr();
            std::cout << data.c_str() << std::endl;
            testDeserialize(data, save);
        }

        {
            auto data = testSerializeKeyAxis();
            std::cout << data.c_str() << std::endl;
            testDeserialize(data, save);
        }
        {
            auto data = testSerializeWASD();
            std::cout << data.c_str() << std::endl;
            testDeserialize(data, save);
        }
        {
            auto data = testSerializeMultiplePress();
            std::cout << data.c_str() << std::endl;
            testDeserialize(data, save);
        }
        {
            auto data = testSerializeDelayPress();
            std::cout << data.c_str() << std::endl;
            testDeserialize(data, save);
        }
    }

    void testContexts()
    {
        auto& insys = getServiceProvider().get<IInputSystem>();
        auto action = insys.addAction("PressX", IInputAction::Type::Trigger, insys.createSignal("pressed", "keyboard", [](IInputSignal* signal)
        {
            signal->Properties().set("key", eastl::string("x"));
        }),
                                      [](IInputSignal* signal)
        {
        });
        action->addContextTag("Tag1");
        if (!action->isContextTag("Tag1"))
        {
            NAU_FAILURE();
        }
        DataBlock block;
        action->serialize(&block);
        testDeserialize(toString(block), false);
        action->removeContextTag("Tag1");
        if (action->isContextTag("Tag1"))
        {
            NAU_FAILURE();
        }
        if (!action->isContextTag(""))
        {
            NAU_FAILURE();
        }
        if (!insys.removeAction(eastl::move(action)))
        {
            NAU_FAILURE();
        }
    }

    void testRuntimeCreation()
    {
        using namespace nau;
        IInputSystem& insys = getServiceProvider().get<IInputSystem>();
        // Mouse left
        insys.addAction("MouseWheelLeft", IInputAction::Type::Trigger, insys.createSignal("pressed", "mouse", [](IInputSignal* signal)
        {
            signal->Properties().set("key", eastl::string("mouse_left"));
        }),
                        [](IInputSignal* signal)
        {
            std::cout << " Mouse left " << signal->getValue() << std::endl;
        });
        // Mouse right
        insys.addAction("MouseWheelLeft", IInputAction::Type::Trigger, insys.createSignal("pressed", "mouse", [](IInputSignal* signal)
        {
            signal->Properties().set("key", eastl::string("mouse_right"));
        }),
                        [](IInputSignal* signal)
        {
            std::cout << " Mouse right " << signal->getValue() << std::endl;
        });
        // Mouse middle
        insys.addAction("MouseWheelLeft", IInputAction::Type::Trigger, insys.createSignal("pressed", "mouse", [](IInputSignal* signal)
        {
            signal->Properties().set("key", eastl::string("mouse_middle"));
        }),
                        [](IInputSignal* signal)
        {
            std::cout << " Mouse middle " << signal->getValue() << std::endl;
        });
        // Mouse wheel
        insys.addAction("Mouse3", IInputAction::Type::Trigger, insys.createSignal("pressed", "mouse", [](IInputSignal* signal)
        {
            signal->Properties().set("key", eastl::string("mouse_3"));
        }),
                        [](IInputSignal* signal)
        {
            std::cout << " Mouse 3 " << signal->getValue() << std::endl;
        });
        // Mouse wheel
        insys.addAction("Mouse4", IInputAction::Type::Trigger, insys.createSignal("pressed", "mouse", [](IInputSignal* signal)
        {
            signal->Properties().set("key", eastl::string("mouse_4"));
        }),
                        [](IInputSignal* signal)
        {
            std::cout << " Mouse 4 " << signal->getValue() << std::endl;
        });
    }

    void addDep(const eastl::string& from, const eastl::string& to, eastl::string& out)
    {
        out += "\n";
        out += from + " -> " + to + ";";
    }

    void dumpSignal(IInputSignal* signal, eastl::string& out)
    {
        for (unsigned i = 0; true; ++i)
        {
            auto* input = signal->getInput(i);
            if (input == nullptr)
            {
                break;
            }
            addDep(signal->getName(), input->getName(), out);
            dumpSignal(input, out);
        }
    }

    void dumpToGraph()
    {
        auto& insys = getServiceProvider().get<IInputSystem>();
        eastl::vector<eastl::shared_ptr<IInputAction>> actions;
        insys.getActions(actions);
        eastl::string out =
            "digraph g"
            "\n{"
            "\nsplines = ortho;"
            "\nnodesep = 0.2 rankdir = \"LR\";"
            "\nnode[shape = box width = .5];";
        for (auto action : actions)
        {
            addDep(action->getName(), action->getSignal()->getName(), out);
            dumpSignal(action->getSignal(), out);
        }
        out += "\n}";
        io::IStreamWriter::Ptr stream = io::createNativeFileStream("c:\\temp\\dump.gv", io::AccessMode::Write, io::OpenFileMode::CreateAlways);
        if (stream)
        {
            auto res = stream->write((const std::byte*)out.c_str(), out.size());
        }
    }
}  // namespace nau::input