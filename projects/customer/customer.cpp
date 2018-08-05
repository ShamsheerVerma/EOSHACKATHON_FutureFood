#include <eosiolib/eosio.hpp>
#include <eosiolib/print.hpp>
using namespace eosio;

class customerreport : public eosio::contract {
  public:
      customerreport(account_name s):
        contract(s), // initialization of the base class for the contract
        _customer(s, s) // initialize the table with code and scope NB! Look up definition of code and scope
      {
      }

      /// @abi action
      void creates(account_name username, const std::string& fullname, std::string& foodname, uint64_t qty, uint64_t itemnumber, std::string& notific ) {
        require_auth(username);
        // Let's make sure the primary key doesn't exist
        // _customer.end() is in a way similar to null and it means that the value isn't found
        eosio_assert(_customer.find(itemnumber) == _customer.end(), "This SSN already exists in the addressbook");
        _customer.emplace(get_self(), [&]( auto& p ) {
           p.fullname = fullname;
           p.foodname =foodname;
           p.qty = qty;
           p.itemnumber = itemnumber;
           p.notific = notific;
        });
      }

      
         
  private: 
    // Setup the struct that represents the row in the table
    /// @abi table customer
    struct supplierdist {
      uint64_t itemnumber; // primary key, social security number
      std::string fullname;
      std::string foodname;
      uint64_t qty;
      std::string notific;

      uint64_t primary_key()const { return itemnumber; }
      uint64_t by_qty()const { return qty; }
      std::string full_name()const { return fullname; }
      std::string food_name()const { return foodname; }
      std::string notific_s()const { return notific; }
      
    };

    // We setup the table:
    /// @abi table
    typedef eosio::multi_index< N(customer), supplierdist, indexed_by<N(qty), const_mem_fun<supplierdist, uint64_t, &supplierdist::by_qty>>>  customer;

    customer _customer;

};

 EOSIO_ABI( customerreport, (creates) )
