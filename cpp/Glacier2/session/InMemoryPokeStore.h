// Copyright (c) ZeroC, Inc.

#include "IPokeStore.h"
#include <list>
#include <map>
#include <string>

namespace Server
{
    /// An in-memory implementation of IPokeStore.
    /// @remark This mock implementation is not thread-safe. A real implementation should support concurrent calls.
    class InMemoryPokeStore : public IPokeStore
    {
    public:
        void saveCollection(const std::string& userId, CatchThemAll::PokemonList pokemon) final;
        CatchThemAll::PokemonList retrieveCollection(const std::string& userId) const final;

    private:
        std::map<std::string, CatchThemAll::PokemonList> _store;
    };
}
