# LargeSystemDesign
## LOAddon BugReport
Следующая программа должна получить доступ к первому выделенному блоку и транспонировать все таблицы в нем:
```C++
  void TransposeSelections(Reference<XFrame>& rxFrame) {
    Reference<XController> xController = rxFrame->getController()->getModel()->getCurrentController();
    Reference<XSelectionSupplier> xSelectionSupplier(xController, UNO_QUERY);

    Any selection = xSelectionSupplier->getSelection();
    Reference<XInterface> oSelection(selection, UNO_QUERY);
    Reference<XIndexAccess> xIndexAccess(oSelection, UNO_QUERY);

    Any index = xIndexAccess->getByIndex(0);
    Reference<XInterface> oIndex(index, UNO_QUERY);
    
    Reference<XTextRange> xTextRange(oIndex, UNO_QUERY);

    Reference<XContentEnumerationAccess> cont(xTextRange, UNO_QUERY);
    Reference<XEnumeration> tables = cont->createContentEnumeration(OUString::createFromAscii("com.sun.star.text.TextTable"));
    while (tables->hasMoreElements()) {
        Any t = tables->nextElement();
        Reference<XTextTable> xTable;
        t >>= xTable;
        TransposeTable(xTable);
    }
}
```
Однако `cont->createContentEnumeration(OUString::createFromAscii("com.sun.star.text.TextTable"))` возвращает пустой объект XEnumeration и получить доступ к таблицам не удается.

LibreOffice 6.0.7.3

LibreOffice SDK 6.0

Ubuntu 18.02
___
