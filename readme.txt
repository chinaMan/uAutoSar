1. critical access

   * disable/enable interrupt
   * close scheck lock (but interrupt enabled)
   * sem


2. configure 

3. state manager

4. memory map

XXX_CODE
XXX_CODE_FAST
XXX_DATA_FAST

5. sync access

  fread(filename, buf, len)   ---->  Fls_Read(addr, buf, len)  



  fread_indication()          <----  Fls_MainFunction()
