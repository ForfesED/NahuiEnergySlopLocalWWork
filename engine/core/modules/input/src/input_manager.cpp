// Copyright 2024 N-GINN LLC. All rights reserved.
// Use of this source code is governed by a BSD-3 Clause license that can be found in the LICENSE file.


#include "input_manager.h"

#include <gainput/gainput.h>

#include "gainput/GainputInputDeviceKeyboard.h"
#include "gainput/GainputInputDeviceMouse.h"
#include "nau/diag/assertion.h"
#include "nau/input.h"

constexpr int keyboardOffset = 0;
constexpr int mouseOffset = static_cast<int>(nau::input::Key::KeyCount_);

namespace nau::input
{
    async::Task<> InputManagerImpl::preInitService()
    {
        m_keyboard = m_inputManager.CreateDevice<gainput::InputDeviceKeyboard>(gainput::InputDevice::AutoIndex, gainput::InputDevice::DV_RAW);
        m_mouse = m_inputManager.CreateDevice<gainput::InputDeviceMouse>();

        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::Escape), m_keyboard, gainput::KeyEscape);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::F1), m_keyboard, gainput::KeyF1);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::F2), m_keyboard, gainput::KeyF2);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::F3), m_keyboard, gainput::KeyF3);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::F4), m_keyboard, gainput::KeyF4);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::F5), m_keyboard, gainput::KeyF5);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::F6), m_keyboard, gainput::KeyF6);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::F7), m_keyboard, gainput::KeyF7);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::F8), m_keyboard, gainput::KeyF8);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::F9), m_keyboard, gainput::KeyF9);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::F10), m_keyboard, gainput::KeyF10);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::F11), m_keyboard, gainput::KeyF11);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::F12), m_keyboard, gainput::KeyF12);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::F13), m_keyboard, gainput::KeyF13);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::F14), m_keyboard, gainput::KeyF14);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::F15), m_keyboard, gainput::KeyF15);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::F16), m_keyboard, gainput::KeyF16);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::F17), m_keyboard, gainput::KeyF17);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::F18), m_keyboard, gainput::KeyF18);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::F19), m_keyboard, gainput::KeyF19);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::Print), m_keyboard, gainput::KeyPrint);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::ScrollLock), m_keyboard, gainput::KeyScrollLock);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::Break), m_keyboard, gainput::KeyBreak);

        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::Space), m_keyboard, gainput::KeySpace);

        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::Apostrophe), m_keyboard, gainput::KeyApostrophe);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::Comma), m_keyboard, gainput::KeyComma);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::Minus), m_keyboard, gainput::KeyMinus);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::Period), m_keyboard, gainput::KeyPeriod);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::Slash), m_keyboard, gainput::KeySlash);

        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::N0), m_keyboard, gainput::Key0);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::N1), m_keyboard, gainput::Key1);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::N2), m_keyboard, gainput::Key2);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::N3), m_keyboard, gainput::Key3);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::N4), m_keyboard, gainput::Key4);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::N5), m_keyboard, gainput::Key5);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::N6), m_keyboard, gainput::Key6);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::N7), m_keyboard, gainput::Key7);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::N8), m_keyboard, gainput::Key8);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::N9), m_keyboard, gainput::Key9);

        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::Semicolon), m_keyboard, gainput::KeySemicolon);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::Less), m_keyboard, gainput::KeyLess);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::Equal), m_keyboard, gainput::KeyEqual);

        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::A), m_keyboard, gainput::KeyA);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::B), m_keyboard, gainput::KeyB);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::C), m_keyboard, gainput::KeyC);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::D), m_keyboard, gainput::KeyD);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::E), m_keyboard, gainput::KeyE);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::F), m_keyboard, gainput::KeyF);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::G), m_keyboard, gainput::KeyG);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::H), m_keyboard, gainput::KeyH);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::I), m_keyboard, gainput::KeyI);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::J), m_keyboard, gainput::KeyJ);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::K), m_keyboard, gainput::KeyK);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::L), m_keyboard, gainput::KeyL);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::M), m_keyboard, gainput::KeyM);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::N), m_keyboard, gainput::KeyN);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::O), m_keyboard, gainput::KeyO);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::P), m_keyboard, gainput::KeyP);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::Q), m_keyboard, gainput::KeyQ);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::R), m_keyboard, gainput::KeyR);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::S), m_keyboard, gainput::KeyS);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::T), m_keyboard, gainput::KeyT);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::U), m_keyboard, gainput::KeyU);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::V), m_keyboard, gainput::KeyV);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::W), m_keyboard, gainput::KeyW);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::X), m_keyboard, gainput::KeyX);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::Y), m_keyboard, gainput::KeyY);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::Z), m_keyboard, gainput::KeyZ);

        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::BracketLeft), m_keyboard, gainput::KeyBracketLeft);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::Backslash), m_keyboard, gainput::KeyBackslash);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::BracketRight), m_keyboard, gainput::KeyBracketRight);

        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::Grave), m_keyboard, gainput::KeyGrave);

        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::Left), m_keyboard, gainput::KeyLeft);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::Right), m_keyboard, gainput::KeyRight);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::Up), m_keyboard, gainput::KeyUp);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::Down), m_keyboard, gainput::KeyDown);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::Insert), m_keyboard, gainput::KeyInsert);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::Home), m_keyboard, gainput::KeyHome);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::Delete), m_keyboard, gainput::KeyDelete);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::End), m_keyboard, gainput::KeyEnd);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::PageUp), m_keyboard, gainput::KeyPageUp);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::PageDown), m_keyboard, gainput::KeyPageDown);

        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::NumLock), m_keyboard, gainput::KeyNumLock);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::KpEqual), m_keyboard, gainput::KeyKpEqual);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::KpDivide), m_keyboard, gainput::KeyKpDivide);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::KpMultiply), m_keyboard, gainput::KeyKpMultiply);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::KpSubtract), m_keyboard, gainput::KeyKpSubtract);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::KpAdd), m_keyboard, gainput::KeyKpAdd);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::KpEnter), m_keyboard, gainput::KeyKpEnter);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::KpInsert), m_keyboard, gainput::KeyKpInsert);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::KpEnd), m_keyboard, gainput::KeyKpEnd);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::KpDown), m_keyboard, gainput::KeyKpDown);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::KpPageDown), m_keyboard, gainput::KeyKpPageDown);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::KpLeft), m_keyboard, gainput::KeyKpLeft);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::KpBegin), m_keyboard, gainput::KeyKpBegin);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::KpRight), m_keyboard, gainput::KeyKpRight);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::KpHome), m_keyboard, gainput::KeyKpHome);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::KpUp), m_keyboard, gainput::KeyKpUp);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::KpPageUp), m_keyboard, gainput::KeyKpPageUp);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::KpDelete), m_keyboard, gainput::KeyKpDelete);

        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::BackSpace), m_keyboard, gainput::KeyBackSpace);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::Tab), m_keyboard, gainput::KeyTab);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::Return), m_keyboard, gainput::KeyReturn);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::CapsLock), m_keyboard, gainput::KeyCapsLock);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::ShiftL), m_keyboard, gainput::KeyShiftL);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::CtrlL), m_keyboard, gainput::KeyCtrlL);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::SuperL), m_keyboard, gainput::KeySuperL);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::AltL), m_keyboard, gainput::KeyAltL);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::AltR), m_keyboard, gainput::KeyAltR);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::SuperR), m_keyboard, gainput::KeySuperR);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::Menu), m_keyboard, gainput::KeyMenu);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::CtrlR), m_keyboard, gainput::KeyCtrlR);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::ShiftR), m_keyboard, gainput::KeyShiftR);

        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::Back), m_keyboard, gainput::KeyBack);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::SoftLeft), m_keyboard, gainput::KeySoftLeft);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::SoftRight), m_keyboard, gainput::KeySoftRight);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::Call), m_keyboard, gainput::KeyCall);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::Endcall), m_keyboard, gainput::KeyEndcall);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::Star), m_keyboard, gainput::KeyStar);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::Pound), m_keyboard, gainput::KeyPound);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::DpadCenter), m_keyboard, gainput::KeyDpadCenter);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::VolumeUp), m_keyboard, gainput::KeyVolumeUp);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::VolumeDown), m_keyboard, gainput::KeyVolumeDown);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::Power), m_keyboard, gainput::KeyPower);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::Camera), m_keyboard, gainput::KeyCamera);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::Clear), m_keyboard, gainput::KeyClear);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::Symbol), m_keyboard, gainput::KeySymbol);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::Explorer), m_keyboard, gainput::KeyExplorer);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::Envelope), m_keyboard, gainput::KeyEnvelope);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::Equals), m_keyboard, gainput::KeyEquals);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::At), m_keyboard, gainput::KeyAt);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::Headsethook), m_keyboard, gainput::KeyHeadsethook);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::Focus), m_keyboard, gainput::KeyFocus);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::Plus), m_keyboard, gainput::KeyPlus);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::Notification), m_keyboard, gainput::KeyNotification);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::Search), m_keyboard, gainput::KeySearch);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::MediaPlayPause), m_keyboard, gainput::KeyMediaPlayPause);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::MediaStop), m_keyboard, gainput::KeyMediaStop);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::MediaNext), m_keyboard, gainput::KeyMediaNext);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::MediaPrevious), m_keyboard, gainput::KeyMediaPrevious);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::MediaRewind), m_keyboard, gainput::KeyMediaRewind);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::MediaFastForward), m_keyboard, gainput::KeyMediaFastForward);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::Mute), m_keyboard, gainput::KeyMute);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::Pictsymbols), m_keyboard, gainput::KeyPictsymbols);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::SwitchCharset), m_keyboard, gainput::KeySwitchCharset);

        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::Forward), m_keyboard, gainput::KeyForward);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::Extra1), m_keyboard, gainput::KeyExtra1);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::Extra2), m_keyboard, gainput::KeyExtra2);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::Extra3), m_keyboard, gainput::KeyExtra3);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::Extra4), m_keyboard, gainput::KeyExtra4);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::Extra5), m_keyboard, gainput::KeyExtra5);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::Extra6), m_keyboard, gainput::KeyExtra6);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::Fn), m_keyboard, gainput::KeyFn);

        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::Circumflex), m_keyboard, gainput::KeyCircumflex);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::Ssharp), m_keyboard, gainput::KeySsharp);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::Acute), m_keyboard, gainput::KeyAcute);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::AltGr), m_keyboard, gainput::KeyAltGr);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::Numbersign), m_keyboard, gainput::KeyNumbersign);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::Udiaeresis), m_keyboard, gainput::KeyUdiaeresis);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::Adiaeresis), m_keyboard, gainput::KeyAdiaeresis);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::Odiaeresis), m_keyboard, gainput::KeyOdiaeresis);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::Section), m_keyboard, gainput::KeySection);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::Aring), m_keyboard, gainput::KeyAring);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::Diaeresis), m_keyboard, gainput::KeyDiaeresis);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::Twosuperior), m_keyboard, gainput::KeyTwosuperior);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::RightParenthesis), m_keyboard, gainput::KeyRightParenthesis);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::Dollar), m_keyboard, gainput::KeyDollar);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::Ugrave), m_keyboard, gainput::KeyUgrave);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::Asterisk), m_keyboard, gainput::KeyAsterisk);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::Colon), m_keyboard, gainput::KeyColon);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::Exclam), m_keyboard, gainput::KeyExclam);

        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::BraceLeft), m_keyboard, gainput::KeyBraceLeft);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::BraceRight), m_keyboard, gainput::KeyBraceRight);
        m_inputMap.MapBool(keyboardOffset + static_cast<int>(nau::input::Key::SysRq), m_keyboard, gainput::KeySysRq);

        m_inputMap.MapBool(mouseOffset + static_cast<int>(nau::input::MouseKey::Button0), m_mouse, gainput::MouseButton0);
        m_inputMap.MapBool(mouseOffset + static_cast<int>(nau::input::MouseKey::Button1), m_mouse, gainput::MouseButton1);
        m_inputMap.MapBool(mouseOffset + static_cast<int>(nau::input::MouseKey::Button2), m_mouse, gainput::MouseButton2);
        m_inputMap.MapBool(mouseOffset + static_cast<int>(nau::input::MouseKey::Button3), m_mouse, gainput::MouseButton3);
        m_inputMap.MapBool(mouseOffset + static_cast<int>(nau::input::MouseKey::Button4), m_mouse, gainput::MouseButton4);
        m_inputMap.MapBool(mouseOffset + static_cast<int>(nau::input::MouseKey::Button5), m_mouse, gainput::MouseButton5);
        m_inputMap.MapBool(mouseOffset + static_cast<int>(nau::input::MouseKey::Button6), m_mouse, gainput::MouseButton6);
        m_inputMap.MapBool(mouseOffset + static_cast<int>(nau::input::MouseKey::Button7), m_mouse, gainput::MouseButton7);
        m_inputMap.MapBool(mouseOffset + static_cast<int>(nau::input::MouseKey::Button8), m_mouse, gainput::MouseButton8);
        m_inputMap.MapBool(mouseOffset + static_cast<int>(nau::input::MouseKey::Button9), m_mouse, gainput::MouseButton9);
        m_inputMap.MapBool(mouseOffset + static_cast<int>(nau::input::MouseKey::Button10), m_mouse, gainput::MouseButton10);
        m_inputMap.MapBool(mouseOffset + static_cast<int>(nau::input::MouseKey::Button11), m_mouse, gainput::MouseButton11);
        m_inputMap.MapBool(mouseOffset + static_cast<int>(nau::input::MouseKey::Button12), m_mouse, gainput::MouseButton12);
        m_inputMap.MapBool(mouseOffset + static_cast<int>(nau::input::MouseKey::Button13), m_mouse, gainput::MouseButton13);
        m_inputMap.MapBool(mouseOffset + static_cast<int>(nau::input::MouseKey::Button14), m_mouse, gainput::MouseButton14);
        m_inputMap.MapBool(mouseOffset + static_cast<int>(nau::input::MouseKey::Button15), m_mouse, gainput::MouseButton15);
        m_inputMap.MapBool(mouseOffset + static_cast<int>(nau::input::MouseKey::Button16), m_mouse, gainput::MouseButton16);
        m_inputMap.MapBool(mouseOffset + static_cast<int>(nau::input::MouseKey::Button17), m_mouse, gainput::MouseButton17);
        m_inputMap.MapBool(mouseOffset + static_cast<int>(nau::input::MouseKey::Button18), m_mouse, gainput::MouseButton18);
        m_inputMap.MapBool(mouseOffset + static_cast<int>(nau::input::MouseKey::Button19), m_mouse, gainput::MouseButton19);
        m_inputMap.MapBool(mouseOffset + static_cast<int>(nau::input::MouseKey::Button20), m_mouse, gainput::MouseButton20);

        m_inputMap.MapFloat(mouseOffset + static_cast<int>(nau::input::MouseKey::AxisX), m_mouse, gainput::MouseAxisX);
        m_inputMap.MapFloat(mouseOffset + static_cast<int>(nau::input::MouseKey::AxisY), m_mouse, gainput::MouseAxisY);
        m_inputMap.MapFloat(mouseOffset + static_cast<int>(nau::input::MouseKey::Wheel), m_mouse, gainput::MouseAxisWheel);
        m_inputMap.MapFloat(mouseOffset + static_cast<int>(nau::input::MouseKey::HWheel), m_mouse, gainput::MouseAxisHWheel);

        return async::Task<>::makeResolved();
    }

    async::Task<> InputManagerImpl::initService()
    {
        return async::Task<>::makeResolved();
    }

    gainput::InputManager& InputManagerImpl::getGainput()
    {
        return (m_inputManager);
    }

    void InputManagerImpl::setScreenResolution(int x, int y)
    {
        m_inputManager.SetDisplaySize(x, y);
    }

    void InputManagerImpl::update()
    {
        m_inputManager.Update();
    }

    void InputManagerImpl::update(float dt)
    {
        m_inputManager.Update(dt);
    }

    bool InputManagerImpl::isKeyboardButtonPressed(int deviceId, nau::input::Key key)
    {
        return m_inputMap.GetBoolWasDown(keyboardOffset + static_cast<int>(key));
    }

    bool InputManagerImpl::isKeyboardButtonHold(int deviceId, nau::input::Key key)
    {
        return m_inputMap.GetBool(keyboardOffset + static_cast<int>(key));
    }

    bool InputManagerImpl::isMouseButtonPressed(int deviceId, nau::input::MouseKey key)
    {
        return m_inputMap.GetBoolIsNew(mouseOffset + static_cast<int>(key));
    }

    bool InputManagerImpl::isMouseButtonReleased(int deviceId, nau::input::MouseKey key)
    {
        return m_inputMap.GetBoolWasDown(mouseOffset + static_cast<int>(key));
    }

    bool InputManagerImpl::isMouseButtonHold(int deviceId, nau::input::MouseKey key)
    {
        return m_inputMap.GetBool(mouseOffset + static_cast<int>(key));
    }

    float InputManagerImpl::getMouseAxisValue(int deviceId, nau::input::MouseKey axis)
    {
        return m_inputMap.GetFloat(mouseOffset + static_cast<int>(axis));
    }

    float InputManagerImpl::getMouseAxisDelta(int deviceId, nau::input::MouseKey axis)
    {
        return m_inputMap.GetFloatDelta(mouseOffset + static_cast<int>(axis));
    }
}  // namespace nau::input
