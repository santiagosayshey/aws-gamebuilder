// #include "Wildcard.hpp"
// #include "entity/Player.hpp"

// class SafeHit: public Wildcard {
//     private:
//         bool used = false;

//     public:
//         virtual ~SafeHit() {}

//         void use(Player& owner, std::vector<Player>& players) override {
//             int playerHandTotal = owner.calculateHandTotal()

//             // Draw the new card
//             Card drawnCard = owner.drawCard();

//             // Add the card onto the player's hand
//             owner.addCard(drawnCard);

//             // check if the current total now is above 21
//             if (owner.playerHandTotal() > 21) {
//                 // if it is, then activate safehit and remove the lastcard drawn
//                 owner.removeCard(drawnCard);
//             }
//         }
// }