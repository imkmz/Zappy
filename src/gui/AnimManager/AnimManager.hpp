/*
** EPITECH PROJECT, 2025
** B-YEP-400-STG-4-1-zappy-arthur.desjonqueres
** File description:
** AnimManager
*/

#ifndef ANIMMANAGER_HPP_
    #define ANIMMANAGER_HPP_

#include "IAnimManager/IAnimManager.hpp"

#include <unordered_map>
#include <algorithm>

namespace Zappy {
    class AnimManager : public IAnimManager{
        public:
            AnimManager(const std::string &mdl);
            ~AnimManager() = default;

            void setState(const State &s) override;
            State getState() const override;
            std::shared_ptr<raylib::Model> getModel() const override;
            void updateAnim() override;
        private:
            std::shared_ptr<raylib::Model> _mdl;
            std::vector<raylib::ModelAnimation> _anim;
            std::unordered_map<State, int> _stateArr;
            State _state;
            int _curFrame;
    };
}

#endif /* !ANIMMANAGER_HPP_ */
