#include <eosiolib/eosio.hpp>
#include <eosiolib/print.hpp>
using namespace eosio;

class donorcompany : public eosio::contract {
  public:
      donorcompany(account_name s):
        contract(s), // initialization of the base class for the contract
        _donors(s, s) // initialize the table with code and scope NB! Look up definition of code and scope
      {
      }

      /// @abi action
      void creates(account_name username, uint64_t abn, const std::string& fullname, std::string& foodname, uint64_t timeleft, uint64_t qty, uint64_t itemnumber ) {
        require_auth(username);
        // Let's make sure the primary key doesn't exist
        // _donors.end() is in a way similar to null and it means that the value isn't found
        eosio_assert(_donors.find(itemnumber) == _donors.end(), "This SSN already exists in the addressbook");
        _donors.emplace(get_self(), [&]( auto& p ) {
           p.abn = abn;
           p.fullname = fullname;
           p.foodname =foodname;
           p.qty = qty;
           p.timeleft = timeleft;
           p.itemnumber = itemnumber;

        });
      }

      /// @abi action
      void update(account_name username, uint64_t abn, const std::string& fullname, std::string& foodname, uint64_t timeleft, uint64_t qty, uint64_t itemnumber ) {
        require_auth(username);

        auto donordist = _donors.find(itemnumber);

        eosio_assert(donordist != _donors.end(), "Record was not found");

        _donors.modify( donordist, username, [&](auto& p){
           p.abn = abn;
           p.fullname = fullname;
           p.foodname = foodname;
           p.qty = qty;
           p.timeleft = timeleft;
           p.itemnumber = itemnumber;

        });
     } 
         
  private: 
    // Setup the struct that represents the row in the table
    /// @abi table donor2
    struct donordist {
      uint64_t itemnumber; // primary key, social security number
      std::string fullname;
      std::string foodname;
      uint64_t qty;
      uint64_t timeleft;
      uint64_t abn;


      uint64_t primary_key()const { return itemnumber; }
      uint64_t by_qty()const { return qty; }
      uint64_t time_left()const { return timeleft; }
      std::string full_name()const { return fullname; }
      std::string food_name()const { return foodname; }
      uint64_t by_abn()const { return abn; }

      
    };

    // We setup the table:
    /// @abi table
    typedef eosio::multi_index< N(donors), donordist, indexed_by<N(timeleft), const_mem_fun<donordist, uint64_t, &donordist::time_left>>>  donors;

    donors _donors;

};

 EOSIO_ABI( donorcompany, (creates) (update) )
