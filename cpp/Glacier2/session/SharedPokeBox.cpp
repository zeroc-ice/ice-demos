
#include "SharedPokeBox.h"

using namespace Server;
using namespace std;

SharedPokeBox::SharedPokeBox(IPokeStorePtr pokeStore, IUserIdResolverPtr userIdResolver)
    : _pokeStore(std::move(pokeStore)),
      _userIdResolver(std::move(userIdResolver))
{
}

CatchThemAll::PokemonList
SharedPokeBox::getInventory(const Ice::Current& current)
{
    return _pokeStore->retrieveCollection(getUserId(current));
}

void
SharedPokeBox::caught(CatchThemAll::PokemonList pokemon, const Ice::Current& current)
{
    string userId = getUserId(current);
    CatchThemAll::PokemonList savedPokemon = _pokeStore->retrieveCollection(userId);

    savedPokemon.insert(savedPokemon.end(), pokemon.begin(), pokemon.end());

    // Sort the collection to make it easier to see the Pokémon that have been caught.
    std::sort(savedPokemon.begin(), savedPokemon.end());
    _pokeStore->saveCollection(std::move(userId), std::move(savedPokemon));
}

void
SharedPokeBox::releaseAll(const Ice::Current& current)
{
    _pokeStore->saveCollection(getUserId(current), {});
}

string
SharedPokeBox::getUserId(const Ice::Current& current) const
{
    auto it = _userIdResolver->getUserId(current.id.name);
    if (it == std::nullopt)
    {
        throw Ice::DispatchException(__FILE__, __LINE__, Ice::ReplyStatus::Unauthorized, "Invalid session token");
    }
    return *it;
}
