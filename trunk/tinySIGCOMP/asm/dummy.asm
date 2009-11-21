   ; RFC 4896
   at (0)
   :udvm_memory_size         pad (2)
   :cycles_per_bit           pad (2)
   :sigcomp_version          pad (2)
   :partial_state_id_length  pad (2)
   :state_length             pad (2)
   :reserved                 pad (2)
   at (64)
   :byte_copy_left           pad (2)
   :byte_copy_right          pad (2)
   :input_bit_order          pad (2)
   :stack_location           pad (2)

   ; Simple loop
   ;       Read a byte
   ;       Output a byte
   ; Until there are no more bytes!

   at (128)
   :start
   INPUT-BYTES (1, byte_copy_left, end)
   OUTPUT (byte_copy_left, 1)
   JUMP (start)

   :end
   END-MESSAGE (0,0,0,0,0,0,0)
