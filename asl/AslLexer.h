
// Generated from Asl.g4 by ANTLR 4.7.1

#pragma once


#include "antlr4-runtime.h"




class  AslLexer : public antlr4::Lexer {
public:
  enum {
    T__0 = 1, T__1 = 2, T__2 = 3, T__3 = 4, T__4 = 5, T__5 = 6, T__6 = 7, 
    ASSIGN = 8, PLUS = 9, MINUS = 10, MUL = 11, DIV = 12, MOD = 13, EQUAL = 14, 
    DIFF = 15, GRT = 16, GRTE = 17, LST = 18, LSTE = 19, NOT = 20, AND = 21, 
    OR = 22, VAR = 23, INT = 24, FLOAT = 25, BOOL = 26, CHAR = 27, ARRAY = 28, 
    OF = 29, IF = 30, THEN = 31, ENDIF = 32, ELSE = 33, WHILE = 34, DO = 35, 
    EWHILE = 36, FUNC = 37, ENDFUNC = 38, READ = 39, WRITE = 40, RETURN = 41, 
    BOOLVAL = 42, ID = 43, INTVAL = 44, FLOATVAL = 45, CHARVAL = 46, STRING = 47, 
    COMMENT = 48, WS = 49
  };

  AslLexer(antlr4::CharStream *input);
  ~AslLexer();

  virtual std::string getGrammarFileName() const override;
  virtual const std::vector<std::string>& getRuleNames() const override;

  virtual const std::vector<std::string>& getChannelNames() const override;
  virtual const std::vector<std::string>& getModeNames() const override;
  virtual const std::vector<std::string>& getTokenNames() const override; // deprecated, use vocabulary instead
  virtual antlr4::dfa::Vocabulary& getVocabulary() const override;

  virtual const std::vector<uint16_t> getSerializedATN() const override;
  virtual const antlr4::atn::ATN& getATN() const override;

private:
  static std::vector<antlr4::dfa::DFA> _decisionToDFA;
  static antlr4::atn::PredictionContextCache _sharedContextCache;
  static std::vector<std::string> _ruleNames;
  static std::vector<std::string> _tokenNames;
  static std::vector<std::string> _channelNames;
  static std::vector<std::string> _modeNames;

  static std::vector<std::string> _literalNames;
  static std::vector<std::string> _symbolicNames;
  static antlr4::dfa::Vocabulary _vocabulary;
  static antlr4::atn::ATN _atn;
  static std::vector<uint16_t> _serializedATN;


  // Individual action functions triggered by action() above.

  // Individual semantic predicate functions triggered by sempred() above.

  struct Initializer {
    Initializer();
  };
  static Initializer _init;
};

