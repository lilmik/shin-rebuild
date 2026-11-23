
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  
                    /* 0x32d90  344  callback_00432d90 */

  if (param_2 != 1) {
    if (param_2 != 2) {
      return;
    }
    iVar4 = 0;
    do {
      iVar3 = probe(iVar1 + 0x20,e.at(4),e.at(8) - iVar4);
      if (iVar3 == 0) {
        e.at(8) = e.at(8) - iVar4;
        e.at(0x4c) = e.at(0x4c) + 2;
        return;
      }
      iVar4 = iVar4 + 1;
    } while (iVar4 < 0x1a);
    iVar4 = e.at(8);
    if (iVar4 <= e.at(0x44)) {
      return;
    }
    do {
      iVar3 = probe(iVar1 + 0x20,e.at(4),iVar4);
      if (iVar3 == 0) {
        e.at(8) = iVar4;
        return;
      }
      iVar4 = iVar4 + -1;
    } while (e.at(0x44) <= iVar4);
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
  iVar3 = e.at(4);
  if (e.at(0x40) < iVar3) {
    do {
      iVar2 = probe(iVar4,iVar3,e.at(0x44));
      if (iVar2 == 0) goto LAB_00432e19;
      iVar3 = iVar3 + -1;
    } while (e.at(0x40) <= iVar3);
  }
  else if (iVar3 < e.at(0x40)) {
    do {
      iVar2 = probe(iVar4,iVar3,e.at(0x44));
      if (iVar2 == 0) goto LAB_00432e19;
      iVar3 = iVar3 + 1;
    } while (iVar3 <= e.at(0x40));
  }
LAB_00432e1c:
  iVar3 = e.at(8);
  if (e.at(0x44) < iVar3) {
    do {
      iVar2 = probe(iVar4,e.at(4),iVar3);
      if (iVar2 == 0) {
        e.at(8) = iVar3;
        return;
      }
      iVar3 = iVar3 + -1;
    } while (e.at(0x44) <= iVar3);
    return;
  }
  if (e.at(0x44) <= iVar3) {
    return;
  }
  do {
    iVar2 = probe(iVar4,e.at(4),iVar3);
    if (iVar2 == 0) {
      e.at(8) = iVar3;
      return;
    }
    iVar3 = iVar3 + 1;
  } while (iVar3 <= e.at(0x44));
  return;
LAB_00432e19:
  e.at(4) = iVar3;
  goto LAB_00432e1c;
