#pragma once

#include <cstdint>
#include <string>

#include "bsoncxx/builder/stream/document.hpp"
#include "bsoncxx/json.hpp"
#include "bsoncxx/oid.hpp"
#include "mongocxx/client.hpp"
#include "mongocxx/database.hpp"
#include "mongocxx/uri.hpp"

#include "character_size.h"

namespace MongoTutorial
{
  constexpr char kMongoDbUri[] = "mongodb://0.0.0.0:27017";
  constexpr char kDatabaseName[] = "learning_mongocxx";
  constexpr char kCollectionName[] = "MarioKartCharacters";

  class MongoDbHandler
  {
  public:
    MongoDbHandler()
        : uri(mongocxx::uri(kMongoDbUri)),
          client(mongocxx::client(uri)),
          db(client[kDatabaseName])
    {
    }

    bool AddCharacterToDb(const std::string &character_name, const CharacterSize &size, const int16_t &wins)
    {
      mongocxx::collection collection = db[kCollectionName];
      auto builder = bsoncxx::builder::stream::document({});
      bsoncxx::document::value doc_to_add =
          builder << "characterName" << character_name
                  << "size" << character_size_to_string[size]
                  << "wins" << wins
                  << bsoncxx::builder::stream::finalize;
      collection.insert_one(doc_to_add.view());
      return true;
    };

    bool UpdateWins(const std::string &character_id)
    {
      mongocxx::collection collection = db[kCollectionName];
      auto builder = bsoncxx::builder::stream::document({});
      bsoncxx::oid document_id(character_id);

      bsoncxx::document::value query_doc =
          builder << "_id" << document_id
                  << bsoncxx::builder::stream::finalize;

      bsoncxx::document::value update_doc =
          builder << "$inc" << bsoncxx::builder::stream::open_document
                  << "wins" << 1 << bsoncxx::builder::stream::close_document
                  << bsoncxx::builder::stream::finalize;

      bsoncxx::stdx::optional<mongocxx::result::update> maybe_result =
          collection.update_one(query_doc.view(), update_doc.view());

      if (maybe_result)
      {
        return maybe_result->modified_count() == 1;
      }

      return false;
    }

    bool RemoveCharacterFromDb(const std::string &character_id)
    {
      mongocxx::collection collection = db[kCollectionName];
      auto builder = bsoncxx::builder::stream::document({});
      bsoncxx::oid document_id(character_id);

      bsoncxx::document::value query_doc =
          builder << "_id" << document_id
                  << bsoncxx::builder::stream::finalize;

      bsoncxx::stdx::optional<mongocxx::result::delete_result> maybe_result =
          collection.delete_one(query_doc.view());

      if (maybe_result)
      {
        return maybe_result->deleted_count() == 1;
      }

      return false;
    }

  private:
    mongocxx::uri uri;
    mongocxx::client client;
    mongocxx::database db;
  };
};