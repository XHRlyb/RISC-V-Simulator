#include<iostream>
#include<cstdio>
#include<cstring>
#include<string>
using namespace std;
typedef unsigned int ui;
typedef long long ll;
int bin[35] = {1,2,4,8,16,32,64,128,
    256,512,1024,2048,4096,8192,16384,32768,
    65536,131072,262144,524288,1048576,2097152,4194304,8388608,
    16777216,33554432,67108864,134217728,268435456,536870912,1073741824,-2147483648};
int all_one[35] = {0,1,3,7,15,31,63,127,255,511,1023,2047,4095,8191,16383,32767,65535,131071,262143,524287,1048575,2097151,4194303,8388607,16777215,33554431,67108863,134217727,268435455,536870911,1073741823,2147483647,-1};
void sign_extend(int &imm, int w) 
{ 
    if (w == 0) return;
    if (imm & bin[w - 1]) 
        imm |= (all_one[32] ^ all_one[w]); 
}
void bin_outp(int x)
{
    for (int i = 31; i >= 0; i--)
        if (x & bin[i]) printf("1");
            else printf("0");
    printf("\n");
}

class Memory_Manage{
public:
    char mem[50000005];
    Memory_Manage(const char* filename)
    {
        //printf("hhhhhhhhhhhhh\n");
        //freopen(filename,"r",stdin);
        //freopen("tmp.out","w",stdout);
        //printf("%s\n", filename);
        char s[15]; int tot;
        while (~scanf("%s", s))
        {
            if (s[0] == '@') 
            {
                tot = 0;
                for (int i = 1; i <= 8; i++)
                {
                    tot = tot * 16 + 
                    ((s[i] >= '0' && s[i] <= '9') ? s[i] - '0' : s[i] - 'A' + 10);
                }
                //printf("~~~~~~~%s~~~~~~~tot=%d~~~~~~~~", s, tot);
            }
            else
            {
                int num0, num1;
                if (s[0] >= '0' && s[0] <= '9') num0 = s[0] - '0';
                    else num0 = s[0] + 10 - 'A';
                if (s[1] >= '0' && s[1] <= '9') num1 = s[1] - '0';
                    else num1 = s[1] + 10 - 'A';
                /*if (tot < 16) printf("%s %d ",s,num0 * 16 + num1);
                */mem[tot++] = char(num0 * 16 + num1);
                //printf("%d,", tot - 1);
                /*if (tot <= 16)
                {
                    int hh = (int(mem[tot - 1]) & all_one[8]);
                    printf("%d\n", hh);
                }*/
            }
            if (tot > 2 && mem[tot] == char(15) && mem[tot - 1] == char(240)
                && mem[tot - 2] == char(5) && mem[tot - 3] == char(19)) break;
        }
        //fclose(stdin);
        //printf("\n\n\n");
        //outp(16);
    }
    /*int IF(int pos, int k) // 从pos+1位开始，取k个 
    { 
        int ret = 0, t1 = pos / 8 + 1, t2 = pos % 8; 
        if (t2 != 0)
        {
            for (int i = 1; i <= 8 - t2 && k > 0; i++, k--)
                ret = ret << 1 | (((int)mem[t1]) & bin[8 - (t2 + i)]);
            t1++;
        }
        while (k / 8) ret = ret << 8 | ((int)mem[t1]), t1++, k -= 8;
        for (int i = 1; i <= k; i++)
            ret = ret << 1 | (((int)mem[t1]) & bin[8 - i]);
        return ret;
    }*/
    void outp(int k)
    {
        printf("~~~~~~~~~~~~~~~~~~~~~~\n");
        for (int i = 16912; i < 16916; i++)
        {
            int hhh = (int(mem[i]) & all_one[8]);
            printf("(%d)", hhh);
            int t1 = hhh % 16, t2 = hhh / 16;
            if (t2 < 10) printf("%d", t2);
                else printf("%c", 55 + t2);
            if (t1 < 10) printf("%d", t1);
                else printf("%c", 55 + t1);
            printf(" ");
        }
        printf("\n~~~~~~~~~~~~~~~~~~~~~~\n");
    }
    int load(int pos, int k) 
    {
        int ret = 0;
        for (int i = k / 8 - 1; i >= 0; i--)
            ret = ((ret << 8) | ((ui)mem[pos + i] & all_one[8]));
        return ret;
        // 还需移动pc
    }
    void store(int pos, int data, int k)
    {
        for (int i = 0; i < k / 8; i++)
            mem[pos + i] = char((data >> (i * 8)) & all_one[8]);
    }
}o("heart.data");
class Instruction{
    /*
    * R:   0 ADD, 1 SLL, 2 SLT, 3 SLTU, 4 XOR,
    *      5 SRL, 6 OR, 7 AND, 8 SUB, 9 SRA    (51)
    * 
    * I:   0 ADDI, 1 SLLI, 2 SLTI, 3 SLTIU, 4 XORI, 
    *      5 SRLI, 6 ORI, 7 ANDI, 8 SRAI,      (19)
    *      9 JALR     (103)
    *      10 LB, 11 LH, 12 LW, 14 LBU, 15 LHU   (3)
    * 
    * S:   0 SB, 1 SH, 2 SW    (35)
    * 
    * U:   0 LUI     (55)
    *      1 AUIPC   (23)
    * 
    * B:   0 BEQ, 1 BNE, 4 BLT, 5 BGE, 6 BLTU, 7 BGEU (99)
    * 
    * J:   JAL   (111)
    */
public :
    char type;
    int id, imm, rd, rs1, rs2, Rd, Rs1, Rs2, Pc;
    Instruction() : type('N') {}
    Instruction(int inst, int _pc) : rd(0), rs1(-1), rs2(-1), imm(0)
    {
        int _id = inst & all_one[7];
        Pc = _pc;
        if (inst == 267388179)
        {
            type = 'D';
            return;
        }
        switch (_id)
        {
            case 51 : 
                type = 'R';
                if ((inst & bin[30]) == 0) 
                    id = ((inst >> 12) & all_one[3]);
                else if (((inst >> 12) & all_one[3]) == 0)
                    id = 8;
                else id = 9;
                rd = ((inst >> 7) & all_one[5]);
                rs1 = ((inst >> 15) & all_one[5]);
                rs2 = ((inst >> 20) & all_one[5]);
                break;
            case 35 :
                type = 'S';
                id = ((inst >> 12) & all_one[3]);
                imm = ((inst >> 7) & all_one[5]);
                imm |= (((inst >> 25) & all_one[7]) << 5);
                sign_extend(imm, 12);
                rs1 = ((inst >> 15) & all_one[5]);
                rs2 = ((inst >> 20) & all_one[5]);
                break;
            case 99 :
                type = 'B';
                id = ((inst >> 12) & all_one[3]);
                imm = ((inst >> 8) & all_one[4]);
                imm |= (((inst >> 25) & all_one[6]) << 4);
                imm |= (((inst >> 7) & 1) << 10);
                imm |= (((inst >> 31) & 1) << 11);
                imm <<= 1;
                sign_extend(imm, 13);
                rd = ((inst >> 7) & all_one[5]);
                rs1 = ((inst >> 15) & all_one[5]);
                rs2 = ((inst >> 20) & all_one[5]);
                break;
            case 111 :
                type = 'J';
                imm = (inst >> 31);
                imm = imm << 8 | ((inst >> 12) & all_one[8]);
                imm = imm << 1 | ((inst >> 20) & 1);
                imm = imm << 10 | ((inst >> 21) & all_one[10]);
                imm <<= 1;
                sign_extend(imm, 21);
                rd = ((inst >> 7) & all_one[5]);
                break;
            case 55 :
                type = 'U'; id = 0;
                imm = ((inst >> 12) << 12);
                rd = ((inst >> 7) & all_one[5]);
                break;
            case 23 :
                type = 'U'; id = 1;
                imm = ((inst >> 12) << 12);
                rd = ((inst >> 7) & all_one[5]);
                break;
            case 19 :
                type = 'I';
                id = ((inst >> 12) & all_one[3]);
                rd = ((inst >> 7) & all_one[5]);
                rs1 = ((inst >> 15) & all_one[5]);
                if (id == 1 || id == 5)
                {
                    imm =  ((inst >> 20) & all_one[5]);
                    //sign_extend(imm, 5);
                    if (imm & bin[4]) 
                    {
                        //imm &= all_one[5]; //?????
                        //imm = 0; /*??*/
                        //printf("ERRORRRRRRRRRRRRRRRRRRRRRRRRRR!!\n");
                        //exit(0);
                    }
                }
                else
                    imm = (inst >> 20);//, sign_extend(imm, 12);
                if (id == 5 && (inst & bin[30])) id = 8;
                break;
            case 103 :
                type = 'I'; id = 9;
                rd = ((inst >> 7) & all_one[5]);
                rs1 = ((inst >> 15) & all_one[5]);
                imm = (inst >> 20);
                //sign_extend(imm, 12);
                break;
            case 3 :
                type = 'I'; id = 10 + ((inst >> 12) & all_one[3]);
                rd = ((inst >> 7) & all_one[5]);
                rs1 = ((inst >> 15) & all_one[5]);
                imm = (inst >> 20);
                //sign_extend(imm, 12);
                break;
            default :
                type = 'E';
                bin_outp(inst);
                printf("\nWRONGGGGGGGGGGGGGGGGGGGGGGGGGGGG!\n");
                //exit(0);
                break;
        }
    }
    //void suc_extend(int &imm) { imm <<= 12; }
    void outp()
    {
        switch (type)
        {
            case 'R' :
                switch (id)
                {
                    case 0 : printf("ADD : "); break;
                    case 1 : printf("SLL : "); break;
                    case 2 : printf("SLT : "); break;
                    case 3 : printf("SLTU : "); break;
                    case 4 : printf("XOR : "); break;
                    case 5 : printf("SRL : "); break;
                    case 6 : printf("OR : "); break;
                    case 7 : printf("AND : "); break;
                    case 8 : printf("SUB : "); break;
                    case 9 : printf("SRA : "); break;
                }
                break;
            case 'I' :
                switch (id)
                {
                    case 0 : printf("ADDI : "); break;
                    case 1 : printf("SLLI : "); break;
                    case 2 : printf("SLTI : "); break;
                    case 3 : printf("SLTIU : "); break;
                    case 4 : printf("XORI : "); break;
                    case 5 : printf("SRLI : "); break;
                    case 6 : printf("ORI : "); break;
                    case 7 : printf("ANDI : "); break;
                    case 8 : printf("SRAI : "); break;
                    case 9 : printf("JALR : "); break;
                    case 10 : printf("LB : "); break;
                    case 11 : printf("LH : "); break;
                    case 12 : printf("LW : "); break;
                    case 14 : printf("LBU : "); break;
                    case 15 : printf("LHU : "); break;
                }
                break;
            case 'S' :
                switch (id)
                {
                    case 0 : printf("SB : "); break;
                    case 1 : printf("SH : "); break;
                    case 2 : printf("SW : "); break;
                }
                break;
            case 'U' :
                switch (id)
                {
                    case 0 : printf("LUI : "); break;
                    case 1 : printf("AUIPC : "); break;
                }
                break;
            case 'B' :
                switch (id)
                {
                    case 0 : printf("BEQ : "); break;
                    case 1 : printf("BNE : "); break;
                    case 4 : printf("BLT : "); break;
                    case 5 : printf("BGE : "); break;
                    case 6 : printf("BLTU : "); break;
                    case 7 : printf("BGEU : "); break;
                }
                break;
            case 'J' :
                printf("JAL : "); break;
        }
        if (rd) printf("  rd : reg[%d]=%d  ", rd, Rd);
        if (~rs1) printf("  rs1 : reg[%d]=%d  ", rs1, Rs1);
        if (~rs2) printf("  rs2 : reg[%d]=%d  ", rs2, Rs2);
        printf("  pc=%d    imm=%d\n", Pc, imm);
    }
};

class RISCV_Simulator{
public:
    int reg[32], pc, reg_IF_ID;
    Instruction reg_ID_EX, reg_EX_MEM, reg_MEM_WB;
    RISCV_Simulator() : pc(0)
    {
        for (int i = 0; i < 32; i++) reg[i] = 0;
        reg_IF_ID = 0;
    }
    void debug()
    {
        printf("-----------------------\n");
        printf("pc = %d\n", pc);
        for (int i = 0; i < 32; i++)
        {
            if (i > 0 && i % 8 == 0) printf("\n");
            printf("reg[%d]=%d, ", i, reg[i]);
        }
        printf("\n-----------------------\n");
    }
    void IF() 
    { 
        int ret = 0; 
        for (int i = 3; i >= 0; i--)
            ret = ((ret << 8) | ((ui)o.mem[pc + i] & all_one[8]));
        reg_IF_ID = ret;
        pc += 4;
    }
    void ID() 
    { 
        reg_ID_EX = Instruction(reg_IF_ID, pc - 4);
        if (reg_ID_EX.type == 'D')
        {
            printf("%d\n", (ui)reg[10] & 255u);
            exit(0);
        }
        reg_ID_EX.Rd = reg[reg_ID_EX.rd];
        if (~reg_ID_EX.rs1) reg_ID_EX.Rs1 = reg[reg_ID_EX.rs1];
        if (~reg_ID_EX.rs2) reg_ID_EX.Rs2 = reg[reg_ID_EX.rs2]; 
    }
    void EX()
    {
        Instruction ins = reg_ID_EX;
        switch (ins.type)
        {
            case 'R':
                switch (ins.id)
                {
                    case 0 : ins.Rd = ins.Rs1 + ins.Rs2; break;
                    case 1 : 
                        ins.Rd = (ui)ins.Rs1 << (ins.Rs2 & all_one[5]); 
                        break;
                    case 2 : ins.Rd = ins.Rs1 < ins.Rs2; break;
                    case 3 : ins.Rd = (ui)ins.Rs1 < (ui)ins.Rs2; break;
                    case 4 : ins.Rd = ins.Rs1 ^ ins.Rs2; break;
                    case 5 :
                        ins.Rd = (ui)ins.Rs1 >> (ins.Rs2 & all_one[5]); 
                        //ins.Rd &= all_one[32 -(ins.Rs2 & all_one[5])];
                        break;
                    case 6 : ins.Rd = ins.Rs1 | ins.Rs2; break;
                    case 7 : ins.Rd = ins.Rs1 & ins.Rs2; break;
                    case 8 : ins.Rd = ins.Rs1 - ins.Rs2; break;
                    case 9 : 
                        ins.Rd = ins.Rs1 >> (ins.Rs2 & all_one[5]); 
                        //sign_extend(ins.Rd, 32 - (ins.Rs2 & all_one[5]));
                        break;
                }
                break;
            case 'I':
                switch (ins.id)
                {
                    case 0 : 
                        ins.Rd = ins.Rs1 + ins.imm; 
                        break;
                    case 1 : ins.Rd = (ui)ins.Rs1 << ins.imm; break;
                    case 2 : ins.Rd = ins.Rs1 < ins.imm; break;
                    case 3 : 
                        if (ins.imm == 1) 
                        {
                            ins.Rd = ins.Rs1 == 0;
                            printf("NEQZ!!!!!!!!\n");
                            exit(0);
                        }
                        else ins.Rd = (ui)ins.Rs1 < (ui)ins.imm;
                        break;
                    case 4 : ins.Rd = ins.Rs1 ^ ins.imm; break;
                    case 5 : 
                        ins.Rd = (ui)ins.Rs1 >> ins.imm; 
                        //ins.Rd &= all_one[32 - ins.imm]; 
                        break;
                    case 6 : ins.Rd = ins.Rs1 | ins.imm; break;
                    case 7 : ins.Rd = ins.Rs1 & ins.imm; break;
                    case 8 : 
                        ins.Rd = ins.Rs1 >> ins.imm; 
                        //sign_extend(ins.Rd, 32 - ins.imm);
                        break;
                    case 9 : 
                        ins.Rd = ins.Pc + 4; 
                        ins.Pc = ((ins.Rs1 + ins.imm) & (-1)); //?????
                        break;
                    default : ins.Pc = ins.Rs1 + ins.imm; break;  //这只是地址！
                }
                break;
            case 'S' :
                ins.Pc = ins.Rs1 + ins.imm; break;  //这只是地址！
            case 'U' :
                if (ins.id == 0) ins.Rd = ins.imm;
                    else ins.Rd = ins.Pc + ins.imm;
                break;
            case 'B' :
            {    bool skip_to = false;
                switch (ins.id)
                {
                    case 0 : skip_to = ins.Rs1 == ins.Rs2; break;
                    case 1 : skip_to = ins.Rs1 != ins.Rs2; break;
                    case 4 : skip_to = ins.Rs1 < ins.Rs2; break;
                    case 5 : skip_to = ins.Rs1 >= ins.Rs2; break;
                    case 6 : skip_to = (ui)ins.Rs1 < (ui)ins.Rs2; break;
                    case 7 : skip_to = (ui)ins.Rs1 >= (ui)ins.Rs2; break;
                }
                if (skip_to) ins.Pc += ins.imm;
                break;}
            case 'J' : 
                ins.Rd = ins.Pc + 4; //?????
                ins.Pc = ins.Pc + ins.imm;
                break;
        }
        reg_EX_MEM = ins;
    }
    void MEM()
    {
        Instruction ins = reg_EX_MEM;
        if (ins.type == 'S')
        {
            switch (ins.id)
            {
                case 0 : o.store(ins.Pc, ins.Rs2, 8); break;
                case 1 : o.store(ins.Pc, ins.Rs2, 16); break;
                case 2 : o.store(ins.Pc, ins.Rs2, 32); break;
            }
        }
        else if (ins.type == 'I')
        {
            switch (ins.id)
            {
                case 10 : ins.Rd = o.load(ins.Pc, 8); sign_extend(ins.Rd, 8); break;
                case 11 : ins.Rd = o.load(ins.Pc, 16); sign_extend(ins.Rd, 16); break;
                case 12 : ins.Rd = o.load(ins.Pc, 32); break;
                case 14 : ins.Rd = o.load(ins.Pc, 8); ins.Rd &= all_one[8]; break;
                case 15 : ins.Rd = o.load(ins.Pc, 16); ins.Rd &= all_one[16]; break;
            }
        }
        reg_MEM_WB = ins;
    }
    void WB()
    {
        Instruction ins = reg_MEM_WB;
        if (ins.rd != 0) reg[ins.rd] = ins.Rd;
        if (ins.type == 'J' || ins.type == 'B' 
        || (ins.type == 'I' && ins.id == 9)) 
            if (pc != ins.Pc + 4) pc = ins.Pc;
    }
}simul;
int main()
{
//    o.outp(0);
    while (1)
    {
//        printf("%d,", simul.pc);
//        bin_outp(simul.reg_IF_ID);
        simul.IF();
//        bin_outp(simul.reg_IF_ID);
        simul.ID();
//        simul.reg_ID_EX.outp(); 
        simul.EX(); 
//        simul.reg_EX_MEM.outp();
        simul.MEM(); 
//        simul.reg_MEM_WB.outp();
        simul.WB();
//        simul.debug();
    }
    return 0;
}