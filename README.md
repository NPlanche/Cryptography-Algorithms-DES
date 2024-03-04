   // Stepts
   // Encryption 
   // 1. convert paintext into binary
   // 2. Plaintext (64 bits) to Initial Permutation
   // 3. Initial Permutation
   // 4. Divide into Left0 and Right0 (32 bits)
   // 5. F function given K1 (48 bits) and Right0 --> result 32bit
   // 6. XOR Left0 and result 32bit
   // 7. Switch Orientation (L0 -> R1 and R0 -> L1)
   // 8. Repeat 5-7 steps with k2 - k16
   // 9. ---> L16 and R16
   // 10. Unite L16 and R16 for Final Permutation
   // 11. Final Permutation 
   // 12. Ciphertext
   // 13. Decryption

