
#include "InMemoryPokeStore.h"

using namespace Server;
using namespace std;

void
InMemoryPokeStore::saveCollection(const string& userId, CatchThemAll::PokemonList pokemon)
{
    if (pokemon.empty())
    {
        _store.erase(userId);
    }
    else
    {
        _store[userId] = std::move(pokemon);
    }
}

CatchThemAll::PokemonList
InMemoryPokeStore::retrieveCollection(const string& userId) const
{
    auto it = _store.find(userId);
    return it == _store.end() ? CatchThemAll::PokemonList{} : it->second;
}
