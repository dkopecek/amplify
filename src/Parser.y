%include {
   #include <build-config.h>
   #include <assert.h>
   #include <vector>
}

%name AmPushParser
%extra_argument { Program* program }

%token_prefix AMPUSH_TOKEN_
%token_type { quex::Token* }
%token_destructor { delete $$; }

%type code { CodeType::Value* }
%destructor code { delete $$; }
%type list_entries { std::vector<CodeType::Value>* }
%destructor list_entries { delete $$; }

%syntax_error {
   throw std::runtime_error("syntax error");
}

%nonassoc TERMINATION UNINITIALIZED NAME BOOLEAN HEXFLOAT FLOAT INTEGER LIST LIST_BEGIN LIST_END STRING STRING_BEGIN STRING_END.

start ::= program.

program ::= code(C). {
   const std::vector<CodeType::Value> code = { *C };
   program->setCode(program->createValue(CodeType::type_number, code));
   delete C;
}

program ::= .

code(C) ::= LIST_BEGIN list_entries(E) LIST_END. {
   LOG(DEBUG) << "New list value";
   C = new CodeType::Value();
   *C = program->createValue(ListType::type_number, *E);
   delete E;
}

list_entries(N) ::= list_entries(E) code(C). {
   LOG(DEBUG) << "Append code";
   if (E == nullptr) {
      E = new std::vector<CodeType::Value>;
   }
   N = E;
   N->push_back(*C);
   delete C;
}

list_entries(E) ::= . {
  E = new std::vector<CodeType::Value>;
}

code(C) ::= INTEGER(I). {
   LOG(DEBUG) << "New integer value";
   std::string s(reinterpret_cast<const char *>(I->get_text().c_str()));
   C = new CodeType::Value();
   *C = program->createValue(IntegerType::type_number, s);
   delete I;
}

code(C) ::= NAME(N). {
   LOG(DEBUG) << "New name value";
   std::string s(reinterpret_cast<const char *>(N->get_text().c_str()));
   C = new CodeType::Value();
   *C = program->createValue(NameType::type_number, s);
   delete N;
}

code(C) ::= BOOLEAN(B). {
   LOG(DEBUG) << "New boolean value";
   std::string s(reinterpret_cast<const char *>(B->get_text().c_str()));
   C = new CodeType::Value();
   *C = program->createValue(BooleanType::type_number, s);
   delete B;
}

code(C) ::= FLOAT(F). {
   LOG(DEBUG) << "New float value";
   std::string s(reinterpret_cast<const char *>(F->get_text().c_str()));
   C = new CodeType::Value();
   *C = program->createValue(FloatType::type_number, s);
   delete F;
}

code(C) ::= HEXFLOAT(F). {
   LOG(DEBUG) << "New float value (hex)";
   std::string s(reinterpret_cast<const char *>(F->get_text().c_str()));
   C = new CodeType::Value();
   *C = program->createValue(FloatType::type_number, s);
   delete F;
}

code(C) ::= STRING_BEGIN STRING(S) STRING_END. {
  std::string s(reinterpret_cast<const char *>(S->get_text().c_str()));  
  C = new CodeType::Value();
  *C = program->createValue(StringType::type_number, s);
  delete S;
}
