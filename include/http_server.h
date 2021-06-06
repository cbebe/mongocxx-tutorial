#pragma once

#include <iostream>
#include <string>

#include "SimpleJSON/json.hpp"
#include "mongocxx/instance.hpp"
#include "served/multiplexer.hpp"
#include "served/net/server.hpp"

#include "mongodb_handler.h"

namespace MongoTutorial
{
  constexpr char kSaveEndpoint[] = "/save";
  constexpr char kWinsEndpoint[] = "/wins";
  constexpr char kDeleteEndpoint[] = "/delete";
  constexpr char kAllCharactersEndpoint[] = "/";

  constexpr char kIpAddress[] = "0.0.0.0";
  constexpr char kPort[] = "5000";
  constexpr char kThreads = 10;

  class HttpServer
  {
  public:
    HttpServer(served::multiplexer multiplexer) : multiplexer(multiplexer)
    {
    }

    auto SaveCharacterToMongoDb()
    {
      return [&](served::response &res, const served::request &req)
      {
        json::JSON req_body = json::JSON::Load(req.body());

        auto maybe_size = string_to_character_size.find(req_body["size"].ToString());

        if (maybe_size == string_to_character_size.end())
        {
          return served::response::stock_reply(400, res);
        }

        MongoDbHandler mhandler;
        bool insert_successful = mhandler.AddCharacterToDb(req_body["characterName"].ToString(),
                                                           maybe_size->second,
                                                           req_body["wins"].ToInt());

        insert_successful ? served::response::stock_reply(200, res) : served::response::stock_reply(500, res);
      };
    }
    auto UpdateWins()
    {
      return [&](served::response &res, const served::request &req)
      {
        json::JSON req_body = json::JSON::Load(req.body());

        MongoDbHandler mhandler;
        bool update_successful = mhandler.UpdateWins(req_body["id"].ToString());

        update_successful ? served::response::stock_reply(200, res)
                          : served::response::stock_reply(404, res);
      };
    }
    auto DeleteHandler()
    {
      return [&](served::response &res, const served::request &req)
      {
        json::JSON req_body = json::JSON::Load(req.body());

        MongoDbHandler mhandler;
        bool delete_successful = mhandler.RemoveCharacterFromDb(req_body["id"].ToString());

        delete_successful ? served::response::stock_reply(200, res)
                          : served::response::stock_reply(404, res);
      };
    }

    auto GetAllCharacters()
    {
      return [&](served::response &res, const served::request &_)
      {
        MongoDbHandler mhandler;
        json::JSON allDocuments = mhandler.GetAllDocuments();
        std::ostringstream stream;
        stream << allDocuments;
        res << stream.str();
      };
    }

    void InitializeEndpoints()
    {
      multiplexer.handle(kSaveEndpoint).post(SaveCharacterToMongoDb());
      multiplexer.handle(kWinsEndpoint).post(UpdateWins());
      multiplexer.handle(kDeleteEndpoint).post(DeleteHandler());
      multiplexer.handle(kAllCharactersEndpoint).get(GetAllCharacters());
    }

    void StartServer()
    {
      mongocxx::instance instance;
      served::net::server server(kIpAddress, kPort, multiplexer);

      std::cout << "Server listening on port " << kPort << "..." << std::endl;

      server.run(kThreads);
    }

  private:
    served::multiplexer multiplexer;
  };
};