# Pipelined CPU Implementation

## 專案說明

1. 設計一個pipelined的cpu模擬器 (可以任何語言來模擬 ex:C & C++)
2. 以stall來解決所有的hazard問題，換言之，需要實作hazard的邏輯判斷
3. 3~4人為一組進行分組
4. 撰寫一份專案報告，約3-4頁
### Input 
為一名為memory.txt的文字檔，裡面為MIPS的組合語言程式

### Output 
請輸出此程式執行結果於一名為result.txt的文字檔

### Instructions

- lw
- sw
- add
- sub
- beq (optional)

### Register Number![148062628-e9c615a6-0991-43fe-86c4-48721ad93c0d]

- 32 registers

### Memory Size

- 32 words

### Initial Values of Memory and Register

- 記憶體中的每個word都是1
- $0暫存器的值為0，其他都是1

## 利用Stall解決各種Hazard

- 當在ID階段發現與前一個指令有相依性時，暫時不再往前執行，等到結果在某個cycle寫入目的地暫存器後，在同一個cycle再完成ID
### Data Hazard
![image](https://user-images.githubusercontent.com/36978795/148062217-2c62cfa8-7822-4de8-9fb3-b8b3e613de19.png)

- 在ID階段發現目前指令為beq時，IF不執行抓取指令的動作，等到beq指令完成ID階段後，到達EXE時，在同一cycle才抓取正確位置的指令

### Control Hazard
![image](https://user-images.githubusercontent.com/36978795/148062439-8f873791-da4e-4896-88aa-7c1572dcd50a.png)

## Input Example
### 1.
![image](https://user-images.githubusercontent.com/36978795/148062597-4600c9d0-6256-4c5a-8eb9-f824fd26048b.png)

### 2.
![image](https://user-images.githubusercontent.com/36978795/148062628-e9c615a6-0991-43fe-86c4-48721ad93c0d.png)

### 3.
![image](https://user-images.githubusercontent.com/36978795/148062658-5f45d559-b0fe-49bf-a29a-fa67b6fdc4c6.png)

### 4.
![image](https://user-images.githubusercontent.com/36978795/148062856-9d01f514-fa06-4218-a445-c9882d29dff5.png)![image](https://user-images.githubusercontent.com/36978795/148062907-67d7f275-d253-4b00-b3e2-82d59843a49a.png)

### 5.
![image](https://user-images.githubusercontent.com/36978795/148063519-db10dffe-9e96-4168-9d31-a246d1a1bdbe.png)

### 6.
![image](https://user-images.githubusercontent.com/36978795/148063558-6ffa57a8-e277-43ee-ab39-20654e801b92.png)

### 7.
![image](https://user-images.githubusercontent.com/36978795/148063618-f3ae83b6-f6e8-4878-b4b4-4c57216673cf.png)

## Output Requirements

- 不同clock cycle時，在CPU中執行的指令狀態 **(顯示各指令在ID、EXE、MEM、WB階段末尚未使用的signal值)**
- 最後顯示
  - 執行該段指令需要多少cycle
  - 記憶體與暫存器執行後的結果
- singal輸出順序: RegDst ALUSrc　Branch MemRead MemWrite　RegWrite MemtoReg
  - ex. lw: EX 01 010 11
### Output for Input [Example 1](#Input-Example)

![image](https://user-images.githubusercontent.com/36978795/148064400-ded2788a-3d04-40c9-8b40-fd6bbab68673.png)

![image](https://user-images.githubusercontent.com/36978795/148064062-55f18238-eb22-4a62-8b40-36e4856e3970.png)

![image](https://user-images.githubusercontent.com/36978795/148064529-fbeb2ffa-ecc4-4fc2-9772-c25a512565b9.png)

### Output for Input [Example 2](#Input-Example)

![image](https://user-images.githubusercontent.com/36978795/148064872-576c2e62-7401-4a9a-8721-1932867b276e.png)

![image](https://user-images.githubusercontent.com/36978795/148064895-9b8c0d53-488c-4a38-9cfd-96315a8c1fda.png)

![image](https://user-images.githubusercontent.com/36978795/148064927-ea3d3aea-d9d1-4101-bb4c-cbb6020fa990.png)

### Output for Input [Example 3](#Input-Example)

![image](https://user-images.githubusercontent.com/36978795/148065014-4640f364-6680-4ab1-8d4a-72a4aa63bdff.png)

![image](https://user-images.githubusercontent.com/36978795/148065056-a23ef912-c922-42ac-a1f0-00b9a966a3ca.png)

![image](https://user-images.githubusercontent.com/36978795/148065070-cb256f77-a2e7-4a13-babb-1b0531e364bc.png)






















