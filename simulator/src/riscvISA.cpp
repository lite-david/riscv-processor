/*
 * riscvISA.cpp
 *
 *  Created on: 7 déc. 2016
 *      Author: Simon Rokicki
 */

#include <isa/riscvISA.h>
#include <string.h>
#include <iomanip>
#include <sstream>

#ifndef __NIOS

#include <strings.h>


const char* riscvNamesOP[8] = {"ADD","SLL", "CMPLT", "CMPLTU", "XOR", "", "OR", "AND"};
const char* riscvNamesOPI[8] = {"ADDi", "SLLi", "SLTi", "CMPLTUi", "XORi", "SRLi", "ORi", "ANDi"};
const char* riscvNamesOPW[8] = {"ADDW","SLLW", "", "", "", "SRW", "", ""};
const char* riscvNamesOPIW[8] = {"ADDWi", "SLLWi", "", "", "", "SRi", "", ""};
const char* riscvNamesLD[8] = {"LDB", "LDH", "LDW", "LDD", "LDBU", "LDHU", "LDWU"};
const char* riscvNamesST[8] = {"STB", "STH", "STW", "STD"};
const char* riscvNamesBR[8] = {"BEQ", "BNE", "", "", "BLT", "BGE", "BLTU", "BGEU"};
const char* riscvNamesMUL[8] = {"MPYLO","MPYHI", "MPYHI", "MPYHI", "DIVHI", "DIVHI", "DIVLO", "DIVLO"};

std::string printDecodedInstrRISCV(uint32 ins){
	ac_int<7, false> opcode = ins.slc<7>(0);
	ac_int<5, false> rs1 = ins.slc<5>(15);
	ac_int<5, false> rs2 = ins.slc<5>(20);
	ac_int<5, false> rd = ins.slc<5>(7);
	ac_int<7, false> funct7 = ins.slc<7>(25);
	ac_int<3, false> funct3 = ins.slc<3>(12);
	ac_int<12, false> imm12_I = ins.slc<12>(20);
	ac_int<12, false> imm12_S = 0;
	imm12_S.set_slc(5, ins.slc<7>(25));
	imm12_S.set_slc(0, ins.slc<5>(7));

	ac_int<12, true> imm12_I_signed = ins.slc<12>(20);
	ac_int<12, true> imm12_S_signed = 0;
	imm12_S_signed.set_slc(0, imm12_S.slc<12>(0));

	ac_int<6, false> shamt = ins.slc<6>(20);
	if (opcode == RISCV_OPIW) //If we are on opiw, shamt only have 5bits
		shamt = rs2;

	//In case of immediate shift instr, as shamt needs one more bit the lower bit of funct7 has to be set to 0
	if (opcode == RISCV_OPI && (funct3 == RISCV_OPI_SLLI || funct3 == RISCV_OPI_SRI))
		funct7[0] = 0;


	ac_int<13, false> imm13 = 0;
	imm13[12] = ins[31];
	imm13.set_slc(5, ins.slc<6>(25));
	imm13.set_slc(1, ins.slc<4>(8));
	imm13[11] = ins[7];

	ac_int<13, true> imm13_signed = 0;
	imm13_signed.set_slc(0, imm13);

	ac_int<32, true> imm31_12 = 0;
	imm31_12.set_slc(12, ins.slc<20>(12));

	ac_int<21, false> imm21_1 = 0;
	imm21_1.set_slc(12, ins.slc<8>(12));
	imm21_1[11] = ins[20];
	imm21_1.set_slc(1, ins.slc<10>(21));
	imm21_1[20] = ins[31];

	ac_int<21, true> imm21_1_signed = 0;
	imm21_1_signed.set_slc(0, imm21_1);


	std::stringstream stream;


	switch (opcode)
	{
	case RISCV_LUI:
		stream << "LUI r" + std::to_string(rd) + " " + std::to_string(imm31_12);
	break;
	case RISCV_AUIPC:
		stream << "AUIPC r" + std::to_string(rd) + " " + std::to_string(imm31_12);
	break;
	case RISCV_JAL:
		if (rd==0)
			stream << "J " + std::to_string(imm31_12);
		else
			stream << "JAL " + std::to_string(imm31_12);
	break;
	case RISCV_JALR:
		if (rd==0)
			stream << "JR " + std::to_string(imm31_12);
		else
			stream << "JALR " + std::to_string(imm12_I_signed);
	break;
	case RISCV_BR:
		stream << riscvNamesBR[funct3];
		stream <<  " r" + std::to_string(rs1) + " r" + std::to_string(rs2) + " " + std::to_string(imm13_signed);
	break;
	case RISCV_LD:
		stream << riscvNamesLD[funct3];
		stream <<  " r" + std::to_string(rd) + " = " + std::to_string(imm12_I_signed) + " (" + std::to_string(rs1) + ")";
	break;
	case RISCV_ST:
		stream << riscvNamesST[funct3];
		stream <<  " r" + std::to_string(rs2) + " = " + std::to_string(imm12_S_signed) + " (" + std::to_string(rs1) + ")";
	break;
	case RISCV_OPI:
		if (funct3 == RISCV_OPI_SRI)
			if (funct7 == RISCV_OPI_SRI_SRLI)
				stream << "SRLi r" + std::to_string(rd) + " = r" + std::to_string(rs1) + ", " + std::to_string(shamt);
			else //SRAI
				stream << "SRAi r" + std::to_string(rd) + " = r" + std::to_string(rs1) + ", " + std::to_string(shamt);
		else if (funct3 == RISCV_OPI_SLLI){
			stream << riscvNamesOPI[funct3];
			stream << " r" + std::to_string(rd) + " = r" + std::to_string(rs1) + ", " + std::to_string(shamt);
		}
		else{
			stream << riscvNamesOPI[funct3];
			stream << " r" + std::to_string(rd) + " = r" + std::to_string(rs1) + ", " + std::to_string(imm12_I_signed);
		}
	break;
	case RISCV_OP:
		if (funct7 == 1){
			stream << riscvNamesMUL[funct3];
			stream << " r" + std::to_string(rd) + " = r" + std::to_string(rs1) + ", r" + std::to_string(rs2);
		}
		else{
			if (funct3 == RISCV_OP_ADD)
				if (funct7 == RISCV_OP_ADD_ADD)
					stream << "ADD r" + std::to_string(rd) + " = r" + std::to_string(rs1) + ", r" + std::to_string(rs2);
				else
					stream << "SUB r" + std::to_string(rd) + " = r" + std::to_string(rs1) + ", r" + std::to_string(rs2);
			else if (funct3 == RISCV_OP_SR)
				if (funct7 == RISCV_OP_SR_SRL)
					stream << "SRL r" + std::to_string(rd) + " = r" + std::to_string(rs1) + ", r" + std::to_string(rs2);
				else //SRA
					stream << "SRA r" + std::to_string(rd) + " = r" + std::to_string(rs1) + ", r" + std::to_string(rs2);
			else{
				stream << riscvNamesOP[funct3];
				stream <<  " r" + std::to_string(rd) + " = r" + std::to_string(rs1) + ", r" + std::to_string(rs2);
			}
		}
	break;
	case RISCV_OPIW:
		if (funct3 == RISCV_OPIW_SRW)
			if (funct7 == RISCV_OPIW_SRW_SRLIW)
				stream << "SRLWi r" + std::to_string(rd) + " = r" + std::to_string(rs1) + ", " + std::to_string(rs2);
			else //SRAI
				stream << "SRAWi r" + std::to_string(rd) + " = r" + std::to_string(rs1) + ", " + std::to_string(rs2);
		else if (funct3 == RISCV_OPIW_SLLIW){
			stream << riscvNamesOPI[funct3];
			stream << " r" + std::to_string(rd) + " = r" + std::to_string(rs1) + ", " + std::to_string(rs2);
		}
		else{
			stream << riscvNamesOPIW[funct3];
			stream << " r" + std::to_string(rd) + " = r" + std::to_string(rs1) + ", " + std::to_string(imm12_I_signed);
		}
	break;
	case RISCV_OPW:
		if (funct7 == 1){
			stream << riscvNamesMUL[funct3];
			stream << "W r" + std::to_string(rd) + " = r" + std::to_string(rs1) + ", r" + std::to_string(rs2);
		}
		else{
			if (funct3 == RISCV_OP_ADD)
				if (funct7 == RISCV_OPW_ADDSUBW_ADDW)
					stream << "ADDW r" + std::to_string(rd) + " = r" + std::to_string(rs1) + ", r" + std::to_string(rs2);
				else
					stream << "SUBW r" + std::to_string(rd) + " = r" + std::to_string(rs1) + ", r" + std::to_string(rs2);
			else if (funct3 == RISCV_OPW_SRW)
				if (funct7 == RISCV_OPW_SRW_SRLW)
					stream << "SRLW r" + std::to_string(rd) + " = r" + std::to_string(rs1) + ", r" + std::to_string(rs2);
				else //SRAW
					stream << "SRAW r" + std::to_string(rd) + " = r" + std::to_string(rs1) + ", r" + std::to_string(rs2);
			else{
				stream << riscvNamesOPW[funct3];
				stream <<  " r" + std::to_string(rd) + " = r" + std::to_string(rs1) + ", r" + std::to_string(rs2);
			}
		}
	break;
	case RISCV_SYSTEM:
		stream << "SYSTEM";
	break;
	default:
		fprintf(stderr,"In default part of switch opcode, instr %x is not handled yet", (int) ins);
	break;
	}

	std::string result(stream.str());
	for (int addedSpace = result.size(); addedSpace < 20; addedSpace++)
		result.append(" ");

	return result;
}
#endif
