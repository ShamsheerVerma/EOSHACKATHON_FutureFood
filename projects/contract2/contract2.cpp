#include <eosiolib/eosio.hpp>
#include <eosiolib/print.hpp>
using namespace eosio;

class donorcompany : public eosio::contract {
  public:
      donorcompany(account_name s):
        contract(s), // initialization of the base class for the contract
        _donor(s, s) // initialize the table with code and scope NB! Look up definition of code and scope
      {
      }

      /// @abi action
      void create(account_name username, uint64_t abn, const std::string& fullname, std::string& foodname, uint64_t timeleft, uint64_t qty, uint64_t itemnumber ) {
        require_auth(username);
        // Let's make sure the primary key doesn't exist
        // _donor.end() is in a way similar to null and it means that the value isn't found
        eosio_assert(_donor.find(abn) == _donor.end(), "This SSN already exists in the addressbook");
        _donor.emplace(get_self(), [&]( auto& p ) {
           p.abn = abn;
           p.fullname = fullname;
	         p.foodname =foodname;
           p.qty = qty;
	         p.timeleft = timeleft;
           p.itemnumber = itemnumber; 
        });
      } 

  private: 
    // Setup the struct that represents the row in the table
    /// @abi table donor
    struct company {
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
    typedef eosio::multi_index< N(donor), company, indexed_by<N(timeleft), const_mem_fun<company, uint64_t, &company::time_left>>>  donor;

    donor _donor;

};

 EOSIO_ABI( donorcompany, (create) )
