
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  
                    /* 0x3530  31  callback_00403530 */

  if (param_2 != 1) {
    if (param_2 != 2) {
      return;
    }
    iVar4 = 0;
    do {
      iVar3 = probe(0,e.at(4),e.at(8) - iVar4);
      if (iVar3 == 0) {
        e.at(8) = e.at(8) - iVar4;
        e.at(0x54) = e.at(0x54) + 2;
        return;
      }
      iVar4 = iVar4 + 1;
    } while (iVar4 < 0x1a);
    iVar4 = e.at(8);
    if (iVar4 <= e.at(0x4c)) {
      return;
    }
    if (iVar4 < e.at(0x4c)) {
      return;
    }
    do {
      iVar3 = probe(0,e.at(4),iVar4);
      if (iVar3 == 0) {
        e.at(8) = iVar4;
        return;
      }
      iVar4 = iVar4 + -1;
    } while (e.at(0x4c) <= iVar4);
    return;
  }
  iVar3 = 0;
  iVar4 = 0;
  do {
    iVar2 = probe(iVar4,e.at(4),e.at(8) - iVar3);
    if (iVar2 == 0) {
      e.at(8) = e.at(8) - iVar3;
      return;
    }
    iVar3 = iVar3 + 1;
  } while (iVar3 < 0x1a);
  iVar3 = e.at(0x48);
  iVar2 = e.at(4);
  if (iVar3 < iVar2) {
    if (iVar3 <= iVar2) {
      do {
        iVar3 = probe(iVar4,iVar2,e.at(0x4c));
        if (iVar3 == 0) goto LAB_004035c1;
        iVar2 = iVar2 + -1;
      } while (e.at(0x48) <= iVar2);
    }
  }
  else if ((iVar2 < iVar3) && (iVar2 <= iVar3)) {
    do {
      iVar3 = probe(iVar4,iVar2,e.at(0x4c));
      if (iVar3 == 0) goto LAB_004035c1;
      iVar2 = iVar2 + 1;
    } while (iVar2 <= e.at(0x48));
  }
LAB_004035c4:
  iVar3 = e.at(0x4c);
  iVar2 = e.at(8);
  if (iVar3 < iVar2) {
    if (iVar3 <= iVar2) {
      do {
        iVar3 = probe(iVar4,e.at(4),iVar2);
        if (iVar3 == 0) {
          e.at(8) = iVar2;
          return;
        }
        iVar2 = iVar2 + -1;
      } while (e.at(0x4c) <= iVar2);
      return;
    }
  }
  else if ((iVar2 < iVar3) && (iVar2 <= iVar3)) {
    do {
      iVar3 = probe(iVar4,e.at(4),iVar2);
      if (iVar3 == 0) {
        e.at(8) = iVar2;
        return;
      }
      iVar2 = iVar2 + 1;
    } while (iVar2 <= e.at(0x4c));
    return;
  }
  return;
LAB_004035c1:
  e.at(4) = iVar2;
  goto LAB_004035c4;
