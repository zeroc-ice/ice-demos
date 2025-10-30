// Copyright (c) ZeroC, Inc.

#ifndef IN_MEMORY_POKE_STORE_H
#define IN_MEMORY_POKE_STORE_H

#include "PokeStore.h"
#include <map>
#include <string>

namespace Server
{
    /// An in-memory implementation of PokeStore.
    /// @remark This mock implementation is not thread-safe. A real implementation should support concurrent calls.
    class InMemoryPokeStore : public PokeStore
    {
    public:
        void saveCollection(const std::string& userId, CatchThemAll::PokemonList pokemon) final;
        [[nodiscard]] CatchThemAll::PokemonList retrieveCollection(const std::string& userId) const final;

    private:
        std::map<std::string, CatchThemAll::PokemonList> _store;
    };
}

#endif
