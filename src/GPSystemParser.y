start ::= system.

system ::= GPSYSTEM_KEYWORD STRING BLOCK_START type_section instruction_section input_section fitness_section generation_section BLOCK_END.

type_section ::= type_section type_decl.
type_section ::= .
type_decl ::= TYPE_KEYWORD STRING.

instruction_section ::= instruction_section instruction_decl.
instruction_section ::= .

instruction_decl ::= INSTRUCTION_KEYWORD STRING.
instruction_decl ::= INSTRUCTION_KEYWORD MATCH_KEYWORD STRING.

generation_section ::= GENERATION_KEYWORD BLOCK_START transform_blocks BLOCK_END.

transform_blocks ::= transform_blocks transform_block.
transform_blocks ::= .

transform_block ::= SELECT_KEYWORD selector count BLOCK_START transform_commands BLOCK_END.

selector ::= PAREN_OPEN SELECTOR_IDENTIFIER COLON SELECTOR_IDENTIFIER PAREN_CLOSE.
selector ::= PAREN_OPEN SELECTOR_IDENTIFIER PAREN_CLOSE.

count ::= COUNT_KEYWORD NUMBER_PERCENT.
count ::= COUNT_KEYWORD NUMBER.
count ::= COUNT_KEYWORD STAR_CHARACTER.

transform_commands ::= transform_commands transform_command.
transform_commands ::= .

transform_command ::= TRANSFORM_KEYWORD STRING.
