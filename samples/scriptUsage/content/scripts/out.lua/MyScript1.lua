--[[ Generated with https://github.com/TypeScriptToLua/TypeScriptToLua ]]
binding = MyNativeBinding:New()
function globalFunction(dt)
    if binding:getKeyboardButtonPressed("A") then
        binding:spawn(10, 20, 30)
        print("Key pressed")
    end
    return ("no press (" .. tostring(dt)) .. ")"
end
