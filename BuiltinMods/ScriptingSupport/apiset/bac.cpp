#include <scriptapi.h>
#include <anticheat.h>

#include "../lazyapi.h"

using namespace Mod::Scripting;

static LazyModuleRegister reg("ez:basic-anti-cheat", "BasicAntiCheat", [](JsObjectWarpper native) -> std::string {
  native["onDetected"] = +[](JsValueRef ref) {
    if (GetJsType(ref) != JsFunction) throw std::runtime_error{"Require function argument"};
    Mod::AntiCheat::GetInstance().AddListener(
        SIG("detected"), [fn{ValueHolder{ref}}](std::string_view name, Mod::PlayerEntry const &target) {
          JsObjectWarpper wrap;
          wrap["name"]    = name;
          wrap["target"]  = target;
          JsValueRef ar[] = {GetUndefined(), *wrap};
          JsCallFunction(*fn, ar, 2, nullptr);
        });
  };
  return R"js(
    export const onDetected = import.meta.native.onDetected;
  )js";
});