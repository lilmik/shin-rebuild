
  int iVar1=0;
  int iVar2;
  int iVar3;
  int iVar4;
  
                    

  if (param_2 == 1) {
    iVar3 = 0;
    iVar4 = iVar1 + 0x14;
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
        p.state=2;
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
          goto joined_r0x00402606;
        }
        iVar3 = iVar3 + -1;
      } while (p.at(0x34) <= iVar3);
    }
    else if (iVar3 < p.at(0x34)) {
LAB_004025d7:
      iVar2 = probe(iVar4,iVar3,p.at(0x38));
      if (iVar2 != 0) goto code_r0x004025e9;
      p.at(4) = iVar3;
      p.at(0x3c) = -p.at(0x3c);
      iVar3 = p.at(0x44) + 1;
      p.at(0x44) = iVar3;
joined_r0x00402606:
      if (iVar3 == 3) {
        p.state=2;
      }
    }
LAB_0040260c:
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
          p.state=2;
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
        p.state=2;
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
      iVar3 = probe(iVar1 + 0x14,p.at(4),p.at(8) - iVar4);
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
        p.state=2;
        return;
      }
      iVar4 = iVar4 + 1;
    } while (iVar4 < 0x1a);
    iVar4 = p.at(8);
    if ((p.at(0x38) < iVar4) && (p.at(0x38) <= iVar4)) {
      while (iVar3 = probe(iVar1 + 0x14,p.at(4),iVar4), iVar3 != 0) {
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
        p.state=2;
      }
    }
  }
  return;
code_r0x004025e9:
  iVar3 = iVar3 + 1;
  if (p.at(0x34) < iVar3) goto LAB_0040260c;
  goto LAB_004025d7;
