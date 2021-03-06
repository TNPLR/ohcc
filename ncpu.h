#ifndef NCPU_H_
#define NCPU_H_
enum instructions {
  LDQ,LDD,LDW,LDB,STQ,STD,STW,STB,LDRQ,LDRD,LDRW,LDRB,
  STRQ,STRD,STRW,STRB,LDI,CMP=0x20,MOV,ADD,SUB,MUL,DIV,
  ADC,SBC,MLC,DVC,AND,ORR,EOR,ROL,ROR,SHL,SHR,JEQ=0x40,
  JNE,JLT,JGT,JLE,JGE,JMP,CALL,RET,INT,PUSHQ=0x50,PUSHD,
  PUSHW,PUSHB,POPQ,POPD,POPW,POPB
};
enum interrupt {
  EXIT=0x80,PRINTF
};
#endif
