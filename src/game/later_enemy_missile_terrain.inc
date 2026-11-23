

  int iVar2;
  int iVar3;
  int iVar4;
  
                    

  if (param_2 == 1) {
    iVar3 = 0;
    iVar4 = 0x14;
    do {
      iVar2 = probe(iVar4,p.at(4),p.at(8) - iVar3);
      if (iVar2 == 0) {
        p.at(8) = p.at(8) - iVar3;
        p.at(0x40) = -p.at(0x40);
        iVar4 = p.at(0x44) + 1;
        p.at(0x44) = iVar4;
        if (iVar4 != 3) {
          return;
        }
        p.owner_state = 2;
        return;
      }
      iVar3 = iVar3 + 1;
    } while (iVar3 < 0x1a);
    iVar3 = p.at(4);
    if (p.at(0x34) < iVar3) {
      do {
        iVar2 = probe(iVar4,iVar3,p.at(0x38));
        if (iVar2 == 0) {
          p.at(4) = iVar3;
          p.at(0x3c) = -p.at(0x3c);
          iVar3 = p.at(0x44) + 1;
          p.at(0x44) = iVar3;
          goto joined_r0x0043ba26;
        }
        iVar3 = iVar3 + -1;
      } while (p.at(0x34) <= iVar3);
    }
    else if (iVar3 < p.at(0x34)) {
LAB_0043b9f7:
      iVar2 = probe(iVar4,iVar3,p.at(0x38));
      if (iVar2 != 0) goto code_r0x0043ba09;
      p.at(4) = iVar3;
      p.at(0x3c) = -p.at(0x3c);
      iVar3 = p.at(0x44) + 1;
      p.at(0x44) = iVar3;
joined_r0x0043ba26:
      if (iVar3 == 3) {
        p.owner_state = 2;
      }
    }
LAB_0043ba2c:
    iVar3 = p.at(0x38);
    iVar2 = p.at(8);
    if (iVar3 < iVar2) {
      if (iVar3 <= iVar2) {
        while (iVar3 = probe(iVar4,p.at(4),iVar2), iVar3 != 0) {
          iVar2 = iVar2 + -1;
          if (iVar2 < p.at(0x38)) {
            return;
          }
        }
        p.at(8) = iVar2;
        p.at(0x40) = -p.at(0x40);
        iVar4 = p.at(0x44) + 1;
        p.at(0x44) = iVar4;
        if (iVar4 == 3) {
          p.owner_state = 2;
          return;
        }
      }
    }
    else if ((iVar2 < iVar3) && (iVar2 <= iVar3)) {
      while (iVar3 = probe(iVar4,p.at(4),iVar2), iVar3 != 0) {
        iVar2 = iVar2 + 1;
        if (p.at(0x38) < iVar2) {
          return;
        }
      }
      p.at(8) = iVar2;
      p.at(0x40) = -p.at(0x40);
      iVar4 = p.at(0x44) + 1;
      p.at(0x44) = iVar4;
      if (iVar4 == 3) {
        p.owner_state = 2;
        return;
      }
    }
  }
  else {
    if (param_2 != 2) {
      return;
    }
    iVar4 = 0;
    do {
      iVar3 = probe(0x14,p.at(4),p.at(8) - iVar4);
      if (iVar3 == 0) {
        p.at(8) = p.at(8) - iVar4;
        iVar4 = p.at(0x40) + 2;
        p.at(0x40) = iVar4;
        p.at(0x40) = -iVar4;
        iVar4 = p.at(0x44) + 1;
        p.at(0x44) = iVar4;
        if (iVar4 != 3) {
          return;
        }
        p.owner_state = 2;
        return;
      }
      iVar4 = iVar4 + 1;
    } while (iVar4 < 0x1a);
    iVar4 = p.at(8);
    if ((p.at(0x38) < iVar4) && (p.at(0x38) <= iVar4)) {
      while (iVar3 = probe(0x14,p.at(4),iVar4), iVar3 != 0) {
        iVar4 = iVar4 + -1;
        if (iVar4 < p.at(0x38)) {
          return;
        }
      }
      p.at(8) = iVar4;
      p.at(0x40) = -p.at(0x40);
      iVar4 = p.at(0x44) + 1;
      p.at(0x44) = iVar4;
      if (iVar4 == 3) {
        p.owner_state = 2;
      }
    }
  }
  return;
code_r0x0043ba09:
  iVar3 = iVar3 + 1;
  if (p.at(0x34) < iVar3) goto LAB_0043ba2c;
  goto LAB_0043b9f7;
