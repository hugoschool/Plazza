#pragma once
#include <functional>
#include <memory>
#include <mutex>
#include <map>
#include <vector>
namespace plazza {

    template<typename T, typename U>
    class SafeMap {
        private:
            std::mutex _mutex;
            std::map<T, U> _map;

        public:
            SafeMap() = default;
            ~SafeMap() = default;

            void insert(T key, U value) {
                std::unique_lock lock(_mutex);

                _map.insert({key, value});
            };

            void erase(T key) {
                std::unique_lock lock(_mutex);

                _map.erase(key);
            };

            U& at(T key) {
                std::unique_lock lock(_mutex);

                return _map.at(key);
            };

            template<typename F>
            void applyAt(T key, std::function<F> fct) {
                std::unique_lock lock(_mutex);

                U &elem = _map.at(key);
                fct(elem);
            };

            size_t size() {
                std::unique_lock lock(_mutex);

                return _map.size();
            };

            bool empty() {
                std::unique_lock lock(_mutex);

                return _map.empty();
            };

            std::_Rb_tree_iterator<std::pair<const T, U>> begin() {
                std::unique_lock lock(_mutex);

                return _map.begin();
            };

            std::vector<T> keys() const {
                std::vector<T> vec;

                for (auto &[key, val]: _map) {
                    vec.push_back(key);
                }
                return vec;
            };
    };
}
