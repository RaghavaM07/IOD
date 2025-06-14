#pragma once

namespace IOD {
    class ICommand {
    public:
        virtual void execute() = 0;
    };
}