/*
Group 10
Name: Hemant Singh Sisodiya ID: 2019A7PS0070P
Name: Akash S Revankar      ID: 2019A7PS0294P
Name: Harsh Butani          ID: 2019A7PS0022P
Name: Siddharth Upadhyay    ID: 2019A7PS0033P
Name: Mohit Sharma          ID: 2019A7PS0100P
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "semantic_analyzer.h"
#include "tree.h"
#include "lexerDef.h"

void declarationVerification(nAryTreeNode *declNode)
{
  nAryTreeNode *idNode = declNode->rightMostChild;
  type *typ = searchHashTablePtr(rootSymbolTablePtr->table, idNode->token.id);
  if (typ != NULL)
  {
    printf(RED "error: line %d: " RESET, declNode->token.line_no);
    errorsDetected = true;
    printf("global variable exists with same name %s\n",idNode->token.id);
  }
}

bool isLoopVariable(nAryTreeNode* whileNode, nAryTreeNode* idNode){
  loopVars* lvs = whileNode->extraArguments;
  if(lvs==NULL){
    return false;
  }
  loopVar* lv = lvs->firstVar;
  while(lv){
    if(strcmp(lv->varName, idNode->token.id)==0){
      return true;
    }
    lv = lv->nextVar;
  }
}

void parameterListVerification(parListNode *param, nAryTreeNode *list)
{
  int line_no=0;
  while (param && list)
  {

    line_no=list->token.line_no;
    type *t1 = param->t;

    if (list->sym.isTerminal == false)
    {
      list = list->leftMostChild;
    }
    type *t2 = findEntryInSymTab(list->scopeSymTab, list->token.id, list->enclFunTypePtr, NULL);


    if(!t1 || !t2){
      printf(RED "error: line %d: " RESET, list->token.line_no);
      errorsDetected = true;
      printf("paramter does not exist\n");
    }
    else if (doTypesMatch(t1, t2) == false)
    {
    printf(RED "error: line %d: " RESET, list->token.line_no);
    errorsDetected = true;
    printf("type mismatch:type of %s is %s in function definition but of %s is %s\n", param->paramName, keyToToken[t1->name],list->token.id, keyToToken[t2->name]);
    }

    list = list->brother;
    param = param->next;
  }

  if (param)
  {
    printf(RED "error: line %d: " RESET, line_no);
    errorsDetected = true;
    printf("number of arguments does not match\n");
    return;
  }
  if (list)
  {
    if (list->sym.isTerminal && list->sym.t == TK_FUNID)
    {
    }
    else
    {
      printf(RED "error: line %d: " RESET, list->token.line_no);
      errorsDetected = true;
      printf("number of arguments does not match\n");
    }
  }
  return;
}

void funCallSemanticsVerification(nAryTreeNode *funCallNode, SymbolTable *currentSymbolTablePtr)
{

  if (funCallNode == NULL)
  {
    return;
  }
  nAryTreeNode *funIdNode = NULL;
  nAryTreeNode *funCallOutputList = NULL;
  nAryTreeNode *funCallInputList = NULL;

  if (funCallNode->numOfChildren == 2)
  {

    funIdNode = funCallNode->leftMostChild;
    funCallOutputList = NULL;
    funCallInputList = funCallNode->rightMostChild;
  }
  else if (funCallNode->numOfChildren == 3)
  {

    funIdNode = funCallNode->leftMostChild->brother;
    funCallOutputList = funCallNode->leftMostChild;
    funCallInputList = funCallNode->rightMostChild;
  }
  else
  {
    printf("something is wrong with you\n");
    return;
  }

  char *enclFunName = funCallNode->enclFunTypePtr->typeInformation.function.functionName;
  char *calledFunName = funIdNode->token.id;
  type *funSymTabEntry = searchHashTablePtr(rootSymbolTablePtr->table, calledFunName);
  if (strcmp(enclFunName, calledFunName) == 0)
  {
    printf(RED "error: line %d: " RESET, funIdNode->token.line_no);
    printf("function can't call itself\n");
    return;
  }
  if (funSymTabEntry == NULL)
  {
    printf(RED "error: line %d: " RESET, funIdNode->token.line_no);
    printf("function %s not defined\n", funIdNode->token.id);
    return;
  }

  paramsList *outputParameterList = funSymTabEntry->typeInformation.function.outputParList;
  parListNode *output_params = NULL;
  if (outputParameterList != NULL)
  {
    output_params = outputParameterList->first;
  }

  parListNode *input_params = funSymTabEntry->typeInformation.function.inputParList->first;

  parListNode *temp = output_params;

  parameterListVerification(output_params, funCallOutputList);
  parameterListVerification(input_params, funCallInputList);

  if (funCallNode->enclFunTypePtr != NULL && funCallOutputList != NULL)
  {
    nAryTreeNode *fncall_list_node=funCallOutputList;
    if(!funCallOutputList->sym.isTerminal){
      fncall_list_node = funCallOutputList->leftMostChild;
    }

    bool is_outp_node = false;
    while (fncall_list_node != NULL)
    {
      findEntryInSymTab(currentSymbolTablePtr, fncall_list_node->token.id, funCallNode->enclFunTypePtr, &is_outp_node);

      type * t=searchHashTablePtr(currentSymbolTablePtr->table,fncall_list_node->token.id);

      if(t!=NULL){
        t->isAssigned=true;
      }

      if (is_outp_node)
        markOutputParameter(fncall_list_node->token.id, funCallNode->enclFunTypePtr);

      nAryTreeNode *while_node = NULL;
      bool isLoopVar=false;
      if (isIdInWhileLoop(fncall_list_node, &while_node, &isLoopVar) == true)
      {
        if(isLoopVariable(while_node, fncall_list_node)){
        
         markLoopVar(while_node, fncall_list_node);
        }
      }

      fncall_list_node = fncall_list_node->brother;

      if(fncall_list_node && fncall_list_node->sym.isTerminal && fncall_list_node->sym.t==TK_FUNID){
        break;
      }
    }
  }
}

void constructSemanticsVerification(nAryTreeNode *root)
{
  if (!root)
  {
    return;
  }

  if (root->sym.isTerminal == false)
  {

    if (root->sym.nt == function)
    {
      funDefSemanticsVerification(root, root->scopeSymTab);
    }
    else if (root->sym.nt == conditionalstmt)
    {
      conditionSemanticsVerification(root, root->scopeSymTab);
    }
    else if (root->sym.nt == assignmentstmt)
    {
      assignmentSemanticsVerification(root, root->scopeSymTab);
    }
    else if (root->sym.nt == funcallstmt)
    {
      funCallSemanticsVerification(root, root->scopeSymTab);
    }
    else if (root->sym.nt == iterativestmt)
    {
      whileLoopSemanticsVerification(root, root->scopeSymTab);
    }
    else if (root->sym.nt == iostmt)
    {
      ioSemanticsVerification(root, root->scopeSymTab);
    }
    else if (root->sym.nt == declaration)
    {
      declarationVerification(root);
    }
  }
}

void thirdASTPass(nAryTreeNode *root)
{

  if (root == NULL)
  {
    return;
  }

  nAryTreeNode *child = root->leftMostChild;


  while (child != NULL)
  {
    thirdASTPass(child);
    child = child->brother;
  }

    constructSemanticsVerification(root);
  return;
}

void insertAlias(nAryTreeNode *node)
{
  if (node->visited == true)
  {
    return;
  }
  nAryTreeNode *aliasNode = node->rightMostChild;
  nAryTreeNode *og_node = getNthChild(node, 2);
  type *og_type_ptr = searchHashTablePtr(rootSymbolTablePtr->table, og_node->token.id);
  if (og_type_ptr == NULL)
  {
    nAryTreeNode *constrTypeNode = searchHashTablePtr(ruidToNode, og_node->token.id);
    if (constrTypeNode == NULL)
    {
      printf(RED "error: line %d: " RESET, aliasNode->token.line_no);
      errorsDetected = true;
      printf("%s not defined\n", og_node->token.id);
      return;
    }
    if (constrTypeNode->sym.nt == definetypestmt)
      og_type_ptr = getType(constrTypeNode->rightMostChild);
    else
      og_type_ptr = getType(getNthChild(constrTypeNode, 2));
  }
  aliases *alias;
  if (og_type_ptr->name == TK_RECORD)
  {
    alias = og_type_ptr->typeInformation.Record.alias;
  }
  else if (og_type_ptr->name == TK_UNION)
  {
    alias = og_type_ptr->typeInformation.Union.alias;
  }
  if (og_type_ptr->name != ALIAS)
  {
    if (alias == NULL)
    {
      alias = (aliases *)malloc(sizeof(aliases));
      strncpy(alias->aliasName, node->rightMostChild->token.id, MAX_LEX_LEN);
      alias->next = NULL;
      if (og_type_ptr->name == TK_RECORD)
        og_type_ptr->typeInformation.Record.alias = alias;
      else
        og_type_ptr->typeInformation.Union.alias = alias;
    }
    else
    {
      while (alias->next)
      {
        alias = alias->next;
      }
      alias->next = (aliases *)malloc(sizeof(aliases));
      strcpy(alias->next->aliasName, node->rightMostChild->token.id);
      alias->next->next = NULL;
    }
  }
  type *alias_type_ptr = (type *)malloc(sizeof(type));
  alias_type_ptr->name = ALIAS;
  strcpy(alias_type_ptr->typeInformation.Alias.aliasOf, og_node->token.id);
  strcpy(alias_type_ptr->typeInformation.Alias.aliasName, aliasNode->token.id);
  alias_type_ptr->line_no = aliasNode->token.line_no;
  alias_type_ptr->width = og_type_ptr->width;
  insertTablePtr(rootSymbolTablePtr->table, aliasNode->token.id, alias_type_ptr);
  node->visited = true;
}

void addUnionFields(type *constr_type, nAryTreeNode *fields)
{
  nAryTreeNode *fieldNode = fields->leftMostChild;
  nAryTreeNode *tNode, *idNode;
  constr_type->typeInformation.Union.members = (paramsList *)malloc(sizeof(paramsList));
  constr_type->typeInformation.Union.members->first = NULL;
  constr_type->typeInformation.Union.members->last = NULL;
  constr_type->typeInformation.Union.members->length = 0;
  line_no = 0;
  parListNode *temp = constr_type->typeInformation.Union.members->first;
  while (fieldNode)
  {
    if(fieldNode->sym.nt == morefields)
      fieldNode = fieldNode->leftMostChild;
    tNode = fieldNode->leftMostChild;
    idNode = fieldNode->rightMostChild;
    type *typ = getType(tNode);
    if (typ == NULL)
    {
      printf(RED "error: line %d: " RESET, tNode->token.line_no);
      errorsDetected = true;
      printf("%s not defined\n", tNode->token.id);
      return;
    }
    line_no = tNode->token.line_no;
    if (temp == NULL)
    {
      temp = (parListNode *)malloc(sizeof(parListNode));
      temp->t = typ;
      strcpy(temp->paramName, idNode->token.id);
      temp->next = NULL;
      temp->t->line_no = line_no;
      constr_type->typeInformation.Union.members->first = temp;
      constr_type->typeInformation.Union.members->last = temp;
    }
    else
    {
      temp->next = (parListNode *)malloc(sizeof(parListNode));
      temp->next->t = typ;
      strcpy(temp->next->paramName, idNode->token.id);
      temp->next->next = NULL;
      temp->next->t->line_no = line_no;
      constr_type->typeInformation.Union.members->last = temp->next;
      temp = temp->next;
    }
    constr_type->width = (constr_type->width > typ->width) ? constr_type->width : typ->width;
    constr_type->typeInformation.Union.members->length++;
    fieldNode = fieldNode->brother;
  }
}

void addRecordFields(type *constr_type, nAryTreeNode *fields)
{

  nAryTreeNode *fieldNode = fields->leftMostChild;
  nAryTreeNode *tNode, *idNode;
  constr_type->typeInformation.Record.members = (paramsList *)malloc(sizeof(paramsList));
  constr_type->typeInformation.Record.members->first = NULL;
  constr_type->typeInformation.Record.members->last = NULL;
  constr_type->typeInformation.Record.members->length = 0;
  int offset = 0;
  parListNode *temp = constr_type->typeInformation.Record.members->first;
  bool tagPresent = false;
  bool tagTypeCorrect = false;
  bool unionPresent = false;
  bool multiUnionPresent = false;
  type *tagType, *variantFieldType;
  int line_no = 0;
  while (fieldNode)
  {
    if(fieldNode->sym.nt == morefields)
      fieldNode = fieldNode->leftMostChild;
    tNode = fieldNode->leftMostChild;
    idNode = fieldNode->rightMostChild;
    line_no = tNode->token.line_no;
    type *typ = getType(tNode);
    if (typ == NULL)
    {
      printf(RED "error: line %d: " RESET, tNode->token.line_no);

      printf("%s not defined\n", tNode->token.id);
      errorsDetected = true;
      return;
    }
    if (typ->name == TK_UNION)
    {
      if (unionPresent)
      {
        multiUnionPresent = true;
      }
      else
      {
        variantFieldType = typ;
        unionPresent = true;
      }
    }
    if (!strcmp(idNode->token.id, "tagvalue"))
    {
      tagPresent = true;
      if (typ->name == TK_INT)
      {
        tagTypeCorrect = true;
        tagType = typ;
      }
    }

    typ->offset = offset;
    if (temp == NULL)
    {
      temp = (parListNode *)malloc(sizeof(parListNode));
      temp->t = typ;
      strcpy(temp->paramName, idNode->token.id);
      temp->t->line_no = line_no;
      temp->next = NULL;
      constr_type->typeInformation.Record.members->first = temp;
      constr_type->typeInformation.Record.members->last = temp;
    }
    else
    {
      temp->next = (parListNode *)malloc(sizeof(parListNode));
      temp->next->t = typ;
      strcpy(temp->next->paramName, idNode->token.id);
      temp->next->next = NULL;
      temp->next->t->line_no = line_no;
      constr_type->typeInformation.Record.members->last = temp->next;
      temp = temp->next;
    }
    offset += typ->width;
    constr_type->width += typ->width;
    constr_type->typeInformation.Record.members->length++;
    fieldNode = fieldNode->brother;
  }
  if (unionPresent)
  {
    if (multiUnionPresent)
    {
      printf(RED "error: line %d: " RESET, constr_type->line_no);
      printf("multiple union fields present\n");
      errorsDetected = true;
    }
    if (tagPresent)
    {
      if (!tagTypeCorrect)
      {
        printf(RED "error: line %d: " RESET, constr_type->line_no);
        printf("tagvalue must be of type \"int\"\n");
        errorsDetected = true;
      }
      else
      {
        constr_type->typeInformation.Record.isTaggedUnion = true;
        constr_type->typeInformation.Record.tag = tagType;
        constr_type->typeInformation.Record.variantField = variantFieldType;
      }
    }
    else
    {
      printf(RED "error: line %d: " RESET, constr_type->line_no);
      printf("tagvalue missing\n");
      errorsDetected = true;
    }
  }
}

bool isAlias(aliases *alias, char *fieldName)
{
  while (alias)
  {
    if (!strcmp(alias->aliasName, fieldName))
      return true;
    alias = alias->next;
  }
  return false;
}

bool doTypesMatch(type *t1, type *t2)
{
  if (t1 == NULL || t2 == NULL)
    return false;

  if (t1->name == TK_RECORD)
  {
    if (t2->name == TK_RECORD)
    {
      if (!strcmp(t1->typeInformation.Record.recName, t2->typeInformation.Record.recName))
        return true;
      else
        return false;
    }
    else if (t2->name == ALIAS)
    {
      if (isAlias(t1->typeInformation.Record.alias, t2->typeInformation.Alias.aliasName))
      {
        return true;
      }
      else
        return false;
    }
    else
      return false;
  }
  else if (t1->name == TK_UNION)
  {
    if (t2->name == TK_UNION)
    {
      if (!strcmp(t1->typeInformation.Union.uniName, t2->typeInformation.Union.uniName))
        return true;
      else
        return false;
    }
    else if (t2->name == ALIAS)
    {
      if (isAlias(t1->typeInformation.Union.alias, t2->typeInformation.Alias.aliasName))
      {
        return true;
      }
      else
        return false;
    }
    else
      return false;
  }
  else if (t1->name == ALIAS)
  {
    if (t2->name == TK_RECORD || t2->name == TK_UNION)
      return doTypesMatch(t2, t1);
    else if (t2->name == ALIAS)
    {

      while (t2->name == ALIAS)
      {
        t2 = searchHashTablePtr(rootSymbolTablePtr->table, t2->typeInformation.Alias.aliasOf);
      }
      return doTypesMatch(t1, t2);
    }
    else
      return false;
  }
  else if (t1->name == TYPE_ERROR || t2->name == TYPE_ERROR)
    return false;
  else
    return (t1->name == t2->name);
}

void markLoopVar(nAryTreeNode *whileNode, nAryTreeNode* varNode)
{
  loopVars *lvs = whileNode->extraArguments;
  loopVar *lv = lvs->firstVar;
  char* var;
  type* t;
  bool isOp = false;
  while (lv!=NULL)
  {
    if(varNode->sym.isTerminal){
      var = varNode->token.id;
      t = findEntryInSymTab(varNode->scopeSymTab,var,varNode->enclFunTypePtr,&isOp);
    }
    if (strcmp(lv->varName, var)==0){
      if(t!=NULL)
        t->isAssigned = true;
      lv->isAssigned = true;
    }
    lv = lv->nextVar;
  }
}

bool isIdInWhileLoop(nAryTreeNode *idNode, nAryTreeNode **whileNode, bool *isLoopVar)
{


  while (idNode)
  {
    if (idNode->parent)
    {
      if (idNode->parent->sym.nt == iterativestmt)
      {
        *whileNode = idNode->parent;
        if (idNode == (*whileNode)->leftMostChild)
          *isLoopVar = true;
        else
          *isLoopVar = false;
        return true;
      }
    }
    idNode = idNode->parent;
  }
  return false;
}

void insertIdInLoopArgs(nAryTreeNode *whileNode, char *var)
{
  bool flag = false;
  if (whileNode->extraArguments == NULL)
  {
    whileNode->extraArguments = (loopVars *)malloc(sizeof(loopVars));
    flag = true;
  }
  loopVars *lvs = whileNode->extraArguments;
  if (flag)
  {
    lvs->count = 0;
    lvs->firstVar = NULL;
  }
  loopVar *lv = lvs->firstVar;
  while (lv)
  {
    if (!strcmp(lv->varName, var))
      return;
    if (lv->nextVar == NULL)
      break;
    lv = lv->nextVar;
  }
  if (lv == NULL)
  {
    lv = (loopVar *)malloc(sizeof(loopVar));
    strcpy(lv->varName, var);
    lv->nextVar = NULL;
    lvs->firstVar = lv;
  }
  else
  {
    lv->nextVar = (loopVar *)malloc(sizeof(loopVar));
    strcpy(lv->nextVar->varName, var);
    lv->nextVar->nextVar = NULL;
  }

  lvs->count++;
}

void whileLoopSemanticsVerification(nAryTreeNode *whileNode, SymbolTable *currentSymbolTablePtr)
{
  boolSemanticsVerification(whileNode->leftMostChild, currentSymbolTablePtr);
  loopVars *lvs = whileNode->extraArguments;
  loopVar *lv = lvs->firstVar;
  if (lv == NULL)
  {
    // error While Loop has no loop variables
  }
  while (lv)
  {
    bool isOp = false;
    type* t = findEntryInSymTab(whileNode->scopeSymTab,lv->varName,whileNode->enclFunTypePtr, &isOp);
    if (lv->isAssigned){
      return;
    }
    else if(t && t->isAssigned){
      return;
    }
    lv = lv->nextVar;
  }
  printf(RED "error: line %d: " RESET, whileNode->token.line_no);
  printf("none of the iteration variables reassigned\n");
  errorsDetected = true;
}

type *getType(nAryTreeNode *node)
{

  if (node == NULL)
  {
    return NULL;
  }
  type *type_ptr = (type *)malloc(sizeof(type));

  if (node->sym.isTerminal)
  {
    if (node->sym.t == TK_RUID)
    {
      type_ptr = searchHashTablePtr(rootSymbolTablePtr->table, node->token.id);
      if (type_ptr == NULL)
      {
        nAryTreeNode *constrTypeNode = searchHashTablePtr(ruidToNode, node->token.id);
        if (constrTypeNode == NULL)
        {
          printf(RED "error: line %d: " RESET, node->token.line_no);
          printf("%s not defined\n", node->token.id);
          errorsDetected = true;
          return NULL;
        }
        if (constrTypeNode->sym.nt == typedefinition)
          insertRecordOrUnion(constrTypeNode);
        else if (constrTypeNode->sym.nt == definetypestmt)
          insertAlias(constrTypeNode);
        type_ptr = searchHashTablePtr(rootSymbolTablePtr->table, node->token.id);
      }

    }
    else
    { 
      if (node->sym.t == TK_INT || node->sym.t == TK_NUM)
        type_ptr->name = TK_INT;
      else
        type_ptr->name = TK_REAL;
    }
  }
  else{
    return getType(node->rightMostChild);
  }

  if (type_ptr->name == TK_INT)
  {
    type_ptr->width = WIDTH_INTEGER;
  }
  else if (type_ptr->name == TK_REAL)
  {
    type_ptr->width = WIDTH_REAL;
  }

  type_ptr->isAssigned = false;

  return type_ptr;
}

void insertRecordOrUnion(nAryTreeNode *node)
{
  if (node->visited == true)
  {
    return;
  }
  nAryTreeNode *typeNode = node->leftMostChild;
  nAryTreeNode *idNode = getNthChild(node, 2);
  type *t = searchHashTablePtr(rootSymbolTablePtr->table, idNode->token.id);
  if (t)
  {
    printf(RED "error: line %d: " RESET, idNode->token.line_no);
    errorsDetected = true;
    printf("%s already defined at %d\n", idNode->token.id, t->line_no);
    errorsDetected = true;
    return;
  }
  t = (type *)malloc(sizeof(type));
  t->name = typeNode->sym.t;
  t->line_no = typeNode->token.line_no;
  switch (t->name)
  {
  case TK_RECORD:
    t->typeInformation.Record.alias = NULL;
    t->typeInformation.Record.members = NULL;
    strcpy(t->typeInformation.Record.recName, idNode->token.id);
    t->typeInformation.Record.currentOffset = 0;
    addRecordFields(t, node->rightMostChild);
    break;
  case TK_UNION:
    t->typeInformation.Union.alias = NULL;
    t->typeInformation.Union.members = NULL;
    strcpy(t->typeInformation.Union.uniName, idNode->token.id);
    t->typeInformation.Union.currentOffset = 0;
    addUnionFields(t, node->rightMostChild);
    break;
  }

  t->isAssigned = false;
  t->enclFunName = NULL;

  insertTablePtr(rootSymbolTablePtr->table, idNode->token.id, t);
  node->visited = true;
}

void insertParameterInList(paramsList *parameterList, type *ty, char *parameterName)
{
  parListNode *param_node = (parListNode *)malloc(sizeof(parListNode));
  strncpy(param_node->paramName, parameterName, MAX_LEX_LEN);
  type* tempT = ty;
  while (tempT->name == ALIAS)
  {
    tempT = searchHashTablePtr(rootSymbolTablePtr->table, tempT->typeInformation.Alias.aliasOf);
  }
  if (tempT->name == TK_UNION)
  {
    printf(RED "error: line %d: " RESET, ty->line_no);
    errorsDetected = true;
    printf("function parameter can't be of union type\n");
  }
  param_node->t = ty;

  param_node->next = NULL;

  if (parameterList->last == NULL)
  {
    parameterList->first = param_node;
    parameterList->last = param_node;
  }
  else
  {
    parameterList->last->next = param_node;
    parameterList->last = param_node;
  }
  parameterList->length++;
}

type *checkEnclFunParams(type *fn_type, char *lexeme, bool *isOutputParameter)
{
  if (fn_type == NULL)
  {
    return NULL;
  }
  parListNode *inputParameter = NULL;
  if (fn_type->typeInformation.function.inputParList)
  {
    inputParameter = fn_type->typeInformation.function.inputParList->first;
  }
  while (inputParameter != NULL)
  {


    if (strcmp(inputParameter->paramName, lexeme) == 0)
    {
      return inputParameter->t;
    }
    inputParameter = inputParameter->next;
  }

  parListNode *outputParameter = NULL;
  if (fn_type->typeInformation.function.outputParList)
  {
    outputParameter = fn_type->typeInformation.function.outputParList->first;
  }
  while (outputParameter != NULL)
  {
    if (strcmp(outputParameter->paramName, lexeme) == 0)
    {
      if (isOutputParameter != NULL)
      {
        *isOutputParameter = true;
      }
      return outputParameter->t;
    }
    outputParameter = outputParameter->next;
  }
  return NULL;
}

void ioSemanticsVerification(nAryTreeNode *ioNode, SymbolTable *currentSymbolTablePtr)
{
  if (ioNode == NULL)
  {
    return;
  }
  nAryTreeNode *ioTypeNode = ioNode->leftMostChild;
  nAryTreeNode *idNode = ioNode->rightMostChild;
  bool isOutputParameter = false;
  
  type *idType = NULL;
  if(idNode->sym.isTerminal)
    idType = findEntryInSymTab(currentSymbolTablePtr, idNode->token.id, idNode->enclFunTypePtr, &isOutputParameter);
  else
    idType = getTypeSingleOrRecId(idNode);
  if (idType == NULL)
  {
    printf(RED"error: line %d: "RESET, idNode->token.line_no);
    if(idNode->sym.isTerminal){
      printf("variable \"%s\" not declared\n", idNode->token.id);
    }
    else 
      printf("constructed type\"%s\" not declared\n", idNode->leftMostChild->token.id);
    return;
  }
  if (ioTypeNode->sym.t == TK_WRITE && idType->isAssigned == false)
  {
    printf(YELLOW"warning: line %d: "RESET, ioTypeNode->token.line_no);
    printf("unassigned variable \"%s\" written\n", idNode->token.id);
    return;
  }
  if (ioTypeNode->sym.t == TK_READ)
  {
    nAryTreeNode * marknode=ioNode->rightMostChild;
    if(!marknode->sym.isTerminal){
      marknode=marknode->leftMostChild;
    }

    type * t=searchHashTablePtr(currentSymbolTablePtr->table,marknode->token.id);

    if(t!=NULL){
      t->isAssigned=true;
    }

    
      markOutputParameter(marknode->token.id, ioNode->enclFunTypePtr);

    nAryTreeNode *whileNode = createTreeNode();
    bool isLoopVar = false;
    if (isIdInWhileLoop(idNode, &whileNode, &isLoopVar))
    {
      if (isLoopVariable(whileNode, idNode))
      {
        markLoopVar(whileNode, idNode);
      }
    }
    idType->isAssigned = true;
  }
}

void markOutputParameter(char *id_str, type *fun_type_ptr)
{

  if (!id_str || !fun_type_ptr)
  {
    return;
  }
  parListNode *currnode = NULL;
  if (fun_type_ptr->typeInformation.function.outputParList)
  {
    currnode = fun_type_ptr->typeInformation.function.outputParList->first;
  }

  while (currnode)
  {

    if (strcmp(currnode->paramName, id_str) == 0)
    {
      currnode->t->isAssigned = true;
    }
    currnode = currnode->next;
  }

  return;
}

bool isRelationalOp(int op)
{
  switch (op)
  {
  case TK_LT:
  case TK_LE:
  case TK_GT:
  case TK_GE:
  case TK_NE:
  case TK_EQ:
    return true;
  default:
    return false;
  }
}

void boolSemanticsVerification(nAryTreeNode *boolNode, SymbolTable *currentSymbolTablePtr)
{
  if (boolNode->numOfChildren == 2)
  {
    boolSemanticsVerification(boolNode->rightMostChild, currentSymbolTablePtr);
  }
  else
  {
    if (isRelationalOp(getNthChild(boolNode, 2)->sym.t))
    { 
      type *op1Type, *op2Type;
      if(boolNode->leftMostChild->sym.isTerminal == false)
        op1Type = getTypeSingleOrRecId(boolNode->leftMostChild);
      else if (boolNode->leftMostChild->sym.t == TK_ID)
        op1Type = findEntryInSymTab(currentSymbolTablePtr, boolNode->leftMostChild->token.id, boolNode->leftMostChild->enclFunTypePtr, NULL);
      else
        op1Type = getType(boolNode->leftMostChild);
      if(boolNode->rightMostChild->sym.isTerminal == false)
        op2Type = getTypeSingleOrRecId(boolNode->rightMostChild);
      else if (boolNode->rightMostChild->sym.t == TK_ID)
        op2Type = findEntryInSymTab(currentSymbolTablePtr, boolNode->rightMostChild->token.id, boolNode->rightMostChild->enclFunTypePtr, NULL);
      else
        op2Type = getType(boolNode->rightMostChild);

      if (op1Type == NULL)
      {
        printf(RED "error: line %d: " RESET, boolNode->token.line_no);
        errorsDetected = true;
        printf("variable %s not declared\n", boolNode->leftMostChild->token.id);
      }
      if (op2Type == NULL)
      {
        printf(RED "error: line %d: " RESET, boolNode->token.line_no);
        errorsDetected = true;
        printf("variable %s not declared\n", boolNode->rightMostChild->token.id);
      }
      else if (doTypesMatch(op1Type, op2Type) == false)
      {
        printf(RED "error: line %d: " RESET, boolNode->token.line_no);
        errorsDetected = true;
        char nullType[] = "NULL";
        printf("type mismatch : %s doesn't match with %s\n", (op1Type) ? keyToToken[op1Type->name] : nullType, (op2Type) ? keyToToken[op2Type->name] : nullType);
      }
    }
    else
    { 
      boolSemanticsVerification(boolNode->leftMostChild, currentSymbolTablePtr);
      boolSemanticsVerification(boolNode->rightMostChild, currentSymbolTablePtr);
    }
  }
}
void conditionSemanticsVerification(nAryTreeNode *condNode, SymbolTable *currentSymbolTablePtr)
{
  nAryTreeNode *boolNode = condNode->leftMostChild;
  boolSemanticsVerification(boolNode, currentSymbolTablePtr);
  constructSemanticsVerification(boolNode->brother);
  nAryTreeNode *stmtsNode = getNthChild(condNode, 3);
  if (stmtsNode != NULL)
  {
    nAryTreeNode *temp = stmtsNode->leftMostChild;
    while (temp)
    {
      constructSemanticsVerification(temp);
      temp = temp->brother;
    }
  }
  nAryTreeNode *elsePartNode = getNthChild(condNode, 4);
  if (elsePartNode != NULL)
  {
    nAryTreeNode *temp = elsePartNode->leftMostChild;
    while (temp)
    {
      constructSemanticsVerification(temp);
      temp = temp->brother;
    }
  }
}

void genConstrTypetoNodeMap(hash_entry *table, nAryTreeNode *root, nAryTreeNode *parent)
{
  if (root == NULL)
    return;

  root->parent = parent;

  nAryTreeNode *temp = root->leftMostChild;
  while (temp)
  {
    genConstrTypetoNodeMap(table, temp, root);
    temp = temp->brother;
  }
  if (parent)
  {
    if (parent->token.line_no == 0 || parent->token.line_no > root->token.line_no)
      parent->token.line_no = root->token.line_no;
  }

  if (root->sym.isTerminal == false)
  {
    if (root->sym.nt == typedefinition)
    {
      nAryTreeNode *temp = searchHashTablePtr(table, getNthChild(root, 2)->token.id);
      if (temp != NULL)
      {
        if (temp->sym.nt == typedefinition)
        {
          printf(RED "error: line %d: " RESET, getNthChild(root, 2)->token.line_no);
          errorsDetected = true;
          printf("%s defined already at %d\n", getNthChild(root, 2)->token.id, getNthChild(temp, 2)->token.line_no);
        }
        if (temp->sym.nt == definetypestmt)
        {
          printf(RED "error: line %d: " RESET, getNthChild(root, 2)->token.line_no);
          errorsDetected = true;
          printf("alias can't be of basic type\n");
        }
        temp->visited = true;
      }
      else
        insertTablePtr(table, getNthChild(root, 2)->token.id, root);
    }
    else if (root->sym.nt == definetypestmt)
    {
      nAryTreeNode *temp = searchHashTablePtr(table, root->rightMostChild->token.id);
      if (temp != NULL)
      {
        if (temp->sym.nt == typedefinition)
        {
          printf(RED "error: line %d: " RESET, getNthChild(root, 2)->token.line_no);
          errorsDetected = true;
          printf("basic type can't be used as an alias\n");
          root->visited = true;
        }
        else if (temp->sym.nt == definetypestmt)
        {
          printf(RED "error: line %d: " RESET, getNthChild(root, 2)->token.line_no);
          errorsDetected = true;
          printf("alias already in use\n");
          root->visited = true;
          temp->visited = true;
        }
      }
      else
        insertTablePtr(table, root->rightMostChild->token.id, root);
    }
  }
}

void constructSymbolTable(nAryTreeNode *root, nAryTreeNode *parent)
{

  if (root == NULL)
  {
    return;
  }

  if (parent)
  {
    root->enclFunTypePtr = parent->enclFunTypePtr;
  }

  if (root->sym.isTerminal == false)
  {

    if (root->sym.nt == function)
    {
      if (root->numOfChildren == 4)                                                                                                                                                                // output_params non null hai
        insertFunctionDefinition(rootSymbolTablePtr, root->leftMostChild->token.id, getNthChild(root, 2)->leftMostChild, getNthChild(root, 3)->leftMostChild, root->leftMostChild->token.line_no); // pass the list heads for input and output types
      else
        insertFunctionDefinition(rootSymbolTablePtr, root->leftMostChild->token.id, getNthChild(root, 2)->leftMostChild, NULL, root->leftMostChild->token.line_no);
    }
    else if (root->sym.nt == mainfunction)
    {
      insertFunctionDefinition(rootSymbolTablePtr, "_main", NULL, NULL, root->leftMostChild->token.line_no); // pass the list heads for input and output types
    }
    else if (root->sym.nt == definetypestmt && root->visited==false)
    {
      insertAlias(root);
    }
    else if (root->sym.nt == typedefinition && root->visited==false)
    {
      insertRecordOrUnion(root);
    }

    else if (root->sym.nt == declaration)
    {

      type *t;

      bool flag = true;

      if(root->sym.isTerminal && !strcmp(root->token.id,"c6")){
        printf("YES I am here\n");
      }

      if (root->leftMostChild->sym.isTerminal == false && root->leftMostChild->sym.nt == constructeddatatype)
      {
        t = getType(root->leftMostChild->rightMostChild);
        t->enclFunName = currentSymbolTablePtr->enclFunName;
      }
      else
      {
        t = getType(root->leftMostChild);
        t->enclFunName = currentSymbolTablePtr->enclFunName;
      }
      if (t == NULL)
      {
        return;
      }
      type* tempT = t;
      while(tempT->name == ALIAS){
    tempT = searchHashTablePtr(rootSymbolTablePtr->table, tempT->typeInformation.Alias.aliasOf);
      }
      if (tempT->name == TK_UNION)
      {
        t->name = TYPE_ERROR;
        printf(RED "error: line %d: " RESET, root->rightMostChild->token.line_no);
        errorsDetected = true;
        printf("variable can't be of union type\n");
      }

      if (root->rightMostChild->sym.t == TK_GLOBAL)
      {
        type *t1;
        if (root->leftMostChild->sym.isTerminal == false && root->leftMostChild->sym.nt == constructeddatatype)
        {
          t1 = searchHashTablePtr(rootSymbolTablePtr->table, root->leftMostChild->brother->token.id);
        }
        else
        {
          t1 = searchHashTablePtr(rootSymbolTablePtr->table, root->leftMostChild->brother->token.id);
        }

        if (t1 != NULL)
        {
          printf(RED "error: line %d: " RESET, root->rightMostChild->token.line_no);
          errorsDetected = true;
          printf("redeclaration of global variable \"%s\" \n", getNthChild(root, 2)->token.id);
          flag = false;
        }

        if (flag)
        {
          t->offset = rootSymbolTablePtr->offset;
          rootSymbolTablePtr->offset += t->width;
          insertTablePtr(rootSymbolTablePtr->table, root->leftMostChild->brother->token.id, t);
        }
      }
      else
      {

        type *t1;
        t1 = NULL;
        if (root->leftMostChild->sym.isTerminal == false && root->leftMostChild->sym.nt == constructeddatatype)
        {
          t1 = searchHashTablePtr(currentSymbolTablePtr->table, root->leftMostChild->rightMostChild->token.id);
        }
        else
        {
          t1 = searchHashTablePtr(currentSymbolTablePtr->table, root->leftMostChild->brother->token.id);
        }
        if (t1 != NULL)
        {
          printf(RED "error: line %d: " RESET, root->rightMostChild->token.line_no);
          errorsDetected = true;
          printf("redeclaration of variable \"%s\" \n", getNthChild(root, 2)->token.id);
          flag = false;
        }

        paramsList *outputParameterList = root->enclFunTypePtr->typeInformation.function.outputParList;
        parListNode *output_param = NULL;
        if (outputParameterList)
        {
          output_param = outputParameterList->first;
        }

        paramsList *inputParameterList = root->enclFunTypePtr->typeInformation.function.inputParList;
        parListNode *input_param = NULL;
        if (inputParameterList != NULL)
        {
          input_param = inputParameterList->first;
        }

        while (input_param != NULL)
        {

          if (strcmp(root->leftMostChild->brother->token.id, input_param->paramName) == 0)
          {
            printf(RED "error: line %d: " RESET, root->rightMostChild->token.line_no);
            errorsDetected = true;
            printf("variable \"%s\" is already part of input parameters\n", getNthChild(root, 2)->token.id);
            flag = false;
          }
          input_param = input_param->next;
        }

        while (output_param != NULL)
        {
          if (strcmp(root->leftMostChild->brother->token.id, output_param->paramName) == 0)
          {
            printf(RED "error: line %d: " RESET, root->rightMostChild->token.line_no);
            errorsDetected = true;
            printf("variable \"%s\" is already part of output parameters\n", getNthChild(root, 2)->token.id);
            flag = false;
          }
          output_param = output_param->next;
        }

        if (flag)
        {
          t->offset = currentSymbolTablePtr->offset;
          currentSymbolTablePtr->offset += t->width;
          insertTablePtr(currentSymbolTablePtr->table, root->leftMostChild->brother->token.id, t);
        }
      }
    }
    else if (root->sym.nt == funcallstmt)
    {
      nAryTreeNode *fnNode = NULL;

      if (root->numOfChildren == 2)
      {
        fnNode = getNthChild(root, 1);
      }
      else
      {
        fnNode = getNthChild(root, 2);
      }

      type *fnType = searchHashTablePtr(rootSymbolTablePtr->table, fnNode->token.id);
      if (fnType == NULL)
      {
        printf(RED "error: line %d: " RESET, fnNode->token.line_no);
        errorsDetected = true;
        printf("function \"%s\" used before definition\n", fnNode->token.id);
      }
    }
    if ((root->sym.nt == function) || (root->sym.nt == mainfunction))
    {
      root->enclFunTypePtr = searchHashTablePtr(rootSymbolTablePtr->table, root->leftMostChild->token.id);

    }

    if ((root->sym.nt == function) || (root->sym.nt == mainfunction))
    {

      SymbolTable *new_sym_tab_ptr = initSymbolTable();

      if (new_sym_tab_ptr == NULL)
      {
        printf("new symbol table could not be created\n");
        return;
      }

      initTable(new_sym_tab_ptr->table);

      new_sym_tab_ptr->enclFunName = root->leftMostChild->token.id;
      if (currentSymbolTablePtr)
      {

        if (currentSymbolTablePtr->leftmostChildSymTab == NULL)
        {
          currentSymbolTablePtr->leftmostChildSymTab = new_sym_tab_ptr;
          currentSymbolTablePtr->rightmostChildSymTab = new_sym_tab_ptr;
        }
        else
        {
          currentSymbolTablePtr->rightmostChildSymTab->brotherSymTab = new_sym_tab_ptr;
          currentSymbolTablePtr->rightmostChildSymTab = new_sym_tab_ptr;
        }
      }
      new_sym_tab_ptr->offset = root->enclFunTypePtr->typeInformation.function.offsetParams;
      new_sym_tab_ptr->parentSymTab = currentSymbolTablePtr;
      currentSymbolTablePtr = new_sym_tab_ptr;
    }
  }
  else
  {

    bool isLoopVar = false;
    nAryTreeNode *whilenode = NULL;

    if (root->sym.t == TK_ID)
    {
      if (isIdInWhileLoop(root, &whilenode, &isLoopVar))
      {

        if (isLoopVar == true)
        {
          insertIdInLoopArgs(whilenode, root->token.id);
        }
      }
    }
  }
  constructSymbolTable(root->leftMostChild, root);

  root->scopeSymTab = currentSymbolTablePtr;

  if (!root->sym.isTerminal)
  {

    if (root->sym.nt == function || root->sym.nt == mainfunction)
    {
      currentSymbolTablePtr = currentSymbolTablePtr->parentSymTab;
    }
  }

  nAryTreeNode *brother = root->brother;
    constructSymbolTable(brother, parent);
}

void insertFunctionDefinition(SymbolTable *table, char *lexeme, nAryTreeNode *inp_par_node_list, nAryTreeNode *out_par_node_list, int line_no)
{
  type *existing_entry = searchHashTablePtr(rootSymbolTablePtr->table, lexeme);
  type *t = (type *)malloc(sizeof(type));
  t->name = FUNCTION;
  t->line_no = line_no;
  if (existing_entry)
  {
    printf(RED "error: line %d: " RESET, line_no);
    errorsDetected = true;
    printf("%s function already defined\n", lexeme);
    return;
  }

  strncpy(t->typeInformation.function.functionName, lexeme, MAX_LEX_LEN);

  if (!strncmp(lexeme, "_main", MAX_LEX_LEN))
  {
    t->typeInformation.function.inputParList = NULL;
    t->typeInformation.function.outputParList = NULL;
  }
  else
  {
    t->typeInformation.function.inputParList = (paramsList *)malloc(sizeof(paramsList));
    t->typeInformation.function.inputParList->first = NULL;
    t->typeInformation.function.inputParList->last = NULL;
    t->typeInformation.function.inputParList->length = 0;

    t->typeInformation.function.outputParList = (paramsList *)malloc(sizeof(paramsList));
    t->typeInformation.function.outputParList->first = NULL;
    t->typeInformation.function.outputParList->last = NULL;
    t->typeInformation.function.outputParList->length = 0;
  }

  t->typeInformation.function.currentOffset = 0;
  t->width = DONT_CARE;
  t->offset = DONT_CARE;
  t->offsetUsed = DONT_CARE;
  if (inp_par_node_list != NULL)
  { 

    char *inp_param_name;
    type *inp_param_type;
    nAryTreeNode *inp_param_node = inp_par_node_list->brother;
    while (inp_param_node != NULL)
    {
      inp_param_node = inp_param_node->leftMostChild;
      if (inp_param_node->sym.t == TK_RECORD || inp_param_node->sym.t == TK_UNION)
        inp_param_node = inp_param_node->brother;

      inp_param_type = getType(inp_param_node);
      if (inp_param_type == NULL)
      {
        printf(RED "error: line %d: " RESET, inp_param_node->token.line_no);
        errorsDetected = true;
        printf("%s not defined\n", inp_param_node->token.id);
        return;
      }
      inp_param_type->line_no = inp_param_node->token.line_no;
      inp_param_type->offset = t->typeInformation.function.offsetParams;
      t->typeInformation.function.offsetParams += inp_param_type->width;

      inp_param_type->enclFunName = lexeme;
      insertParameterInList(t->typeInformation.function.inputParList, inp_param_type, inp_param_node->brother->token.id);
      inp_param_node = inp_param_node->brother->brother;
    }
  }
  if (out_par_node_list != NULL)
  { 
    char *out_param_name;
    type *out_param_type;
    nAryTreeNode *out_param_node = out_par_node_list->brother;
    while (out_param_node != NULL)
    {
      out_param_node = out_param_node->leftMostChild;
      if (out_param_node->sym.t == TK_RECORD || out_param_node->sym.t == TK_UNION)
        out_param_node = out_param_node->brother;
      out_param_type = getType(out_param_node);
      if (out_param_type == NULL)
      {
        printf(RED "error: line %d: " RESET, out_param_node->token.line_no);
        errorsDetected = true;
        printf("%s not defined\n", out_param_node->token.id);
        return;
      }
      out_param_type->line_no = out_param_node->token.line_no;
      out_param_type->offset = t->typeInformation.function.offsetParams;
      t->typeInformation.function.offsetParams += out_param_type->width;

      out_param_type->enclFunName = lexeme;
      insertParameterInList(t->typeInformation.function.outputParList, out_param_type, out_param_node->brother->token.id);
      out_param_node = out_param_node->brother->brother;
    }
  }

  t->enclFunName = NULL;
  insertTablePtr(rootSymbolTablePtr->table, lexeme, t);
}

void funDefSemanticsVerification(nAryTreeNode *node, SymbolTable *curr_sym_table_ptr)
{

  if(!node){
    return;
  }


  nAryTreeNode *inp_para_list = node->leftMostChild->brother->leftMostChild->brother;
  type *inp_list_node_type_ptr = NULL;

  while (1)
  {

    if (inp_para_list == NULL)
    {
      break;
    }
    inp_para_list = inp_para_list->leftMostChild->brother;
    inp_list_node_type_ptr = (type *)searchHashTablePtr(rootSymbolTablePtr->table, inp_para_list->token.id);
    if (inp_list_node_type_ptr != NULL)
    {
      printf(RED "error: line %d: " RESET, inp_para_list->token.line_no);
      errorsDetected = true;
      printf("global variable \"%s\" cant' be part of input parameters\n", inp_para_list->token.id);
    }


    inp_para_list = inp_para_list->brother;
  }
  if (node->numOfChildren != 4)
  {
    return;
  }
  nAryTreeNode *out_par_list = node->leftMostChild->brother->brother->leftMostChild->brother;
  type *out_list_node_type_ptr = NULL;
  while (1)
  {
    if (!out_par_list)
    {
      break;
    }

    out_par_list = out_par_list->leftMostChild->brother;

    out_list_node_type_ptr = (type *)searchHashTablePtr(rootSymbolTablePtr->table, out_par_list->token.id);

    if (out_list_node_type_ptr != NULL)
    {
      printf(RED "error: line %d: " RESET, out_par_list->token.line_no);
      errorsDetected = true;
      printf("global variable \"%s\" cant' be part of output parameters\n", out_par_list->token.id);
    }

    out_list_node_type_ptr = (type *)findEntryInSymTab(curr_sym_table_ptr, out_par_list->token.id, node->enclFunTypePtr, NULL);
    if (out_list_node_type_ptr && out_list_node_type_ptr->isAssigned == false)
    {
      printf(RED "error: line %d: " RESET, out_par_list->token.line_no);
      errorsDetected = true;
      printf("output parameter \"%s\" not assigned any value\n", out_par_list->token.id);
    }

    out_par_list = out_par_list->brother;
  }
}

void *findEntryInSymTab(SymbolTable *symTable, char *lexeme, type *enclFnTypePtr, bool *isOutputParameter)
{
  if (!symTable)
  {
    return NULL;
  }
  type *typePtr=NULL;
  typePtr = (type *)searchHashTablePtr(rootSymbolTablePtr->table, lexeme);
  if (typePtr == NULL)
  {
    typePtr = searchHashTablePtr(symTable->table, lexeme);
  }
  if (typePtr == NULL)
  {
    typePtr = checkEnclFunParams(enclFnTypePtr, lexeme, isOutputParameter);
  }
  return typePtr;
}

type *getTypeSingleOrRecId(nAryTreeNode *root)
{
  nAryTreeNode *temp = root->rightMostChild;
  bool isOutputParameter = false;
  type* t = findEntryInSymTab(root->scopeSymTab, root->leftMostChild->token.id, root->enclFunTypePtr, &isOutputParameter);
  if (t == NULL)
  {
    printf(RED "error: line %d: " RESET, root->token.line_no);
    errorsDetected = true;
    printf("%s not defined\n", root->leftMostChild->token.id);
    return NULL;
  }

  while (t->name == ALIAS)
  {
    t = searchHashTablePtr(rootSymbolTablePtr->table, t->typeInformation.Alias.aliasOf);
  }
  while (temp)
  {
    if (temp->sym.isTerminal == false)
      temp = temp->leftMostChild;
    parListNode* mem = NULL;
    if(t->name == TK_RECORD)
      mem = t->typeInformation.Record.members->first;
    else if(t->name == TK_UNION)
      mem = t->typeInformation.Union.members->first;
    bool flag = false;
    while (mem)
    {
      if (strcmp(mem->paramName, temp->token.id) == 0)
      {
        flag = true;
        break;
      }
      mem = mem->next;
    }
    if (flag == false)
    {
      printf(RED "error: line %d: " RESET, root->token.line_no);
      errorsDetected = true;
      printf("\"%s\" is not a member of \"%s\"\n", temp->token.id, t->typeInformation.Record.recName);
      return NULL;
    }
    if(mem!=NULL)
      t = mem->t;
    while (t->name == ALIAS)
    {
      t = searchHashTablePtr(rootSymbolTablePtr->table, t->typeInformation.Alias.aliasOf);
    }
    temp = temp->brother;
  }

  if(!t){
    printf("returning the null\n");
  }
  return t;
}

void assignmentSemanticsVerification(nAryTreeNode *assignNode, SymbolTable *currentSymbolTablePtr)
{
  if (assignNode == NULL)
  {
    return;
  }

  type *expressionType = getArithmeticExpressionType(assignNode->rightMostChild);

  if (!expressionType)
  {
    return;
  }

  type *leftChildType = NULL;
  nAryTreeNode *singleOrRecId = assignNode->leftMostChild;
  bool isOutputParameter = false;
  if (singleOrRecId->sym.isTerminal)
  {
    leftChildType = (type *)findEntryInSymTab(currentSymbolTablePtr, singleOrRecId->token.id, singleOrRecId->enclFunTypePtr, &isOutputParameter);

    if (leftChildType == NULL)
    {
      printf(RED "error: line %d: " RESET, singleOrRecId->token.line_no);
      errorsDetected = true;
      printf("variable \"%s\" not declared\n", singleOrRecId->token.id);
      return;
    }

    if (isOutputParameter == true)
    {
      markOutputParameter(singleOrRecId->token.id, singleOrRecId->enclFunTypePtr);
    }
  }
  else
  {
    leftChildType = getTypeSingleOrRecId(singleOrRecId);
    if (leftChildType == NULL)
    {
      return;
    }
    markOutputParameter(singleOrRecId->leftMostChild->token.id, singleOrRecId->leftMostChild->enclFunTypePtr);
  }

  if (doTypesMatch(leftChildType, expressionType) == false)
  {
    printf(RED "error: line %d: " RESET, singleOrRecId->token.line_no);
    errorsDetected = true;
    char nullType[] = "NULL";

    printf("type mismatch : LHS type %s doesn't match with RHS type %s\n", (leftChildType) ? keyToToken[leftChildType->name] : nullType, (expressionType) ? keyToToken[expressionType->name] : nullType);
  }
  nAryTreeNode *whileNode = NULL;
  bool isLoopVar = false;
  if (singleOrRecId->sym.isTerminal )
  {
    if ( singleOrRecId->sym.t == TK_ID && isIdInWhileLoop(singleOrRecId, &whileNode, &isLoopVar) == true)
    {
      if (isLoopVariable(whileNode, singleOrRecId))
      {
        markLoopVar(whileNode, singleOrRecId);
      }
    }
  }
  else
  {
    nAryTreeNode *temp = singleOrRecId;
    if (isIdInWhileLoop(temp->leftMostChild, &whileNode, &isLoopVar))
    {
      if(isLoopVariable(whileNode, temp->leftMostChild))
      markLoopVar(whileNode, temp->leftMostChild);
    }
  }
  leftChildType->isAssigned = true;

}

bool scalarWithRecord(type *t1, type *t2, bool is_divide)
{
  if (is_divide)
  {
    if ((t2->name == TK_INT || t2->name == TK_REAL) && (t1->name == TK_RECORD || t1->name == TK_UNION || t1->name == ALIAS))
    {
      return false;
    }
    return false;
  }
  if ((t1->name == TK_RECORD || t1->name == TK_UNION || t1->name == ALIAS) && (t2->name == TK_INT || t2->name == TK_REAL))
  {
    return true;
  }

  if ((t2->name == TK_RECORD || t2->name == TK_UNION || t2->name == ALIAS) && (t1->name == TK_INT || t1->name == TK_REAL))
  {
    return true;
  }

  return false;
}

type *getArithmeticExpressionType(nAryTreeNode *root)
{

  if (!root)
  {
    return NULL;
  }
  if (root->sym.isTerminal == true)
  {
    if (root->sym.t == TK_NUM || root->sym.t == TK_RNUM)
    {
      type *t = getType(root);
      return t;
    }

    type *t = findEntryInSymTab(root->scopeSymTab, root->token.id, root->enclFunTypePtr, NULL);
    if (t == NULL)
    {
      printf(RED "error: line %d: " RESET, root->token.line_no);
      errorsDetected = true;
      printf("variable %s not declared\n",root->token.id);
    }

    return t;
  }
  else
  {

    if (root->sym.nt == arithmeticexpression)
    { // term ,exprime

      type *t1 = getArithmeticExpressionType(root->leftMostChild);
      type *t2 = getArithmeticExpressionType(root->rightMostChild);

      if (t1 == NULL || t2 == NULL)
      {
        return NULL;
      }
      if (doTypesMatch(t1, t2) == true)
      {
        return t1;
      }
      else
      {
        printf(RED "error: line %d: " RESET, root->token.line_no);
        errorsDetected = true;
        printf("on RHS %s type does not match with %s,will give TYPE ERROR\n", keyToToken[t1->name], keyToToken[t2->name]);
        type *t = (type *)malloc(sizeof(type));
        t->name = TYPE_ERROR;
        strcpy(t->typeInformation.errorMsg, "type mismatch");
        return t;
      }
    }
    else if (root->sym.nt == term)
    {
      type *t1 = getArithmeticExpressionType(root->leftMostChild);
      type *t2 = getArithmeticExpressionType(root->rightMostChild);
      if(!t1 || !t2){
          return NULL;
      }


      bool is_divide = false;
      if (root->rightMostChild->leftMostChild->sym.t == TK_DIV)
      {
        is_divide = true;
      }

      if (scalarWithRecord(t1, t2, is_divide) == true)
      { 
        if (t1->name == TK_RECORD || t1->name == TK_UNION || t1->name == ALIAS)
        {
          return t1;
        }

        return t2;
      }

      if (doTypesMatch(t1, t2))
      {

        if (t1->name == TK_RECORD || t1->name == ALIAS || t1->name == TK_UNION)
        {
          type *t = (type *)malloc(sizeof(type));
          t->name = TYPE_ERROR;
          strcpy(t->typeInformation.errorMsg, "type mismatch");
          return t;
        }
        if (is_divide)
        {
          type *t = (type *)malloc(sizeof(type));
          t->name = TK_REAL;
          return t;
        }
        else
        {
          return t1;
        }
      }
      else if (is_divide && ((t1->name == TK_INT && t2->name == TK_REAL) || (t2->name == TK_INT && t1->name == TK_REAL)))
      {
        type *t = (type *)malloc(sizeof(type));
        t->name = TK_REAL;
        return t;
      }
      else
      {
        type *t = (type *)malloc(sizeof(type));
        t->name = TYPE_ERROR;
        strcpy(t->typeInformation.errorMsg, "type mismatch");
        return t;
      }
    }
    else if (root->sym.nt == expprime)
    {
      if (root->numOfChildren == 2)
      {
        type *t = getArithmeticExpressionType(root->rightMostChild);
        return t;
      }
      else
      { // 3 children
        type *t1 = getArithmeticExpressionType(root->rightMostChild);
        type *t2 = getArithmeticExpressionType(root->leftMostChild->brother);

        if(!t1 || !t2){
            return NULL;
        }

        if (doTypesMatch(t1, t2))
        {
          return t1;
        }
        type *t = (type *)malloc(sizeof(type));
        t->name = TYPE_ERROR;
        strcpy(t->typeInformation.errorMsg, "type mismatch");
        return t;
      }
    }
    else if (root->sym.nt == termprime)
    {

      if (root->numOfChildren == 2)
      {
        type *t = getArithmeticExpressionType(root->rightMostChild);
        return t;
      }
      else
      {

        type *t1 = getArithmeticExpressionType(root->rightMostChild);
        type *t2 = getArithmeticExpressionType(root->leftMostChild->brother);


        if(!t1 || !t2){
            return NULL;
        }

        bool is_divide = false;
        if (root->leftMostChild->sym.t == TK_DIV)
        {
          is_divide = true;
        }

        if (scalarWithRecord(t1, t2, is_divide) == true)
        {
          if (t1->name == TK_RECORD || t1->name == TK_UNION || t1->name == ALIAS)
          {
            return t1;
          }

          return t2;
        }

        if (doTypesMatch(t1, t2))
        {

          if (t1->name == TK_RECORD || t1->name == ALIAS || t1->name == TK_UNION)
          {
            type *t = (type *)malloc(sizeof(type));
            t->name = TYPE_ERROR;
            strcpy(t->typeInformation.errorMsg, "type mismatch");
            return t;
          }

          if (is_divide)
          {
            type *t = (type *)malloc(sizeof(type));
            t->name = TK_REAL;
            return t;
          }
          else
          {
            return t1;
          }
        }
        else if (is_divide && ((t1->name == TK_INT && t2->name == TK_REAL) || (t2->name == TK_INT && t1->name == TK_REAL)))
        {
          type *t = (type *)malloc(sizeof(type));
          t->name = TK_REAL;
          return t;
        }
        else
        {
          type *t = (type *)malloc(sizeof(type));
          t->name = TYPE_ERROR;
          strcpy(t->typeInformation.errorMsg, "type mismatch");
          return t;
        }
      }
    }
    else if (root->sym.nt == singleorrecid)
    {
      return getTypeSingleOrRecId(root);
    }
    else
      return NULL;
  }
}

void printAlias(type *t)
{
  aliases *alias = t->typeInformation.Record.alias;
  while (alias)
  {
    printf("%s", alias->aliasName);
    alias = alias->next;
    if (alias)
    {
      printf(", ");
    }
  }
}

void printTypeExpression(type *t)
{
  while(t->name == ALIAS){
    t = searchHashTablePtr(rootSymbolTablePtr->table, t->typeInformation.Alias.aliasOf);
  }

  paramsList *members = NULL;
  if(t->name == TK_RECORD)
    members = t->typeInformation.Record.members;
  else if(t->name == TK_UNION)
    members = t->typeInformation.Union.members;
  parListNode *mem = members->first;
  printf("<");
  while (mem)
  {
    type *temp = mem->t;
    if (!temp)
    {
      printf("(null)");
    }
    else if (temp->name == TK_INT)
    {
      printf("int");
    }
    else if (temp->name == TK_REAL)
    {
      printf("real");
    }
    else
      printTypeExpression(temp);

    mem = mem->next;
    if (mem)
    {
      printf(", ");
    }
  }
  printf(">");
}

void printActivationRecord(SymbolTable *sym_table)
{

  if (!sym_table)
  {
    return;
  }

  bool is_global = false;
  if (sym_table != rootSymbolTablePtr)
  {
    printf("%s %d\n", sym_table->enclFunName, sym_table->offset);
  }

  printActivationRecord(sym_table->leftmostChildSymTab);

  SymbolTable *temp = sym_table->brotherSymTab;
    printActivationRecord(temp);
}

void printRecordTypesAndSizes(SymbolTable *sym_table)
{

  if (!sym_table)
  {
    return;
  }

  for (int i = 0; i < HASHTABLE_SIZE; i++)
  {
    hash_entry entry = sym_table->table[i];

    if (entry.present == true)
    {
      type *t = entry.value;

      if (t->name == TK_RECORD)
      {
        printf("%s   ", t->typeInformation.Record.recName);
        printTypeExpression(t);
        printf("    ");
        printf("%d\n", t->width);
      }
    }
  }

}


  void printGLobalVariables(SymbolTable * sym_table)
  {

    for (int i = 0; i < HASHTABLE_SIZE; i++)
    {
      hash_entry entry = sym_table->table[i];

      if (entry.present == true)
      {
        type *t = entry.value;
        char *ID = entry.lexeme;

        if (ID != NULL && (ID[0] != '#' && ID[0] != '_'))
        { 

          if (t->name == TK_INT)
          {
            printf("%s    %s    %d    ", ID, keyToToken[t->name], t->offset);
          }
          else if (t->name == TK_REAL)
          {
            printf("%s    %s    %d    ", ID, keyToToken[t->name], t->offset);
          }
          else if (t->name == TK_RECORD)
          {
            printf("%s    %s    ", ID, t->typeInformation.Record.recName);
            printf("%d    ", t->typeInformation.Record.currentOffset);
          }
          printf("\n");
        }
      }
    }
  }

  void printSymbolTable(SymbolTable * sym_table)
  {

    if (!sym_table)
    {
      return;
    }
    bool is_global = false;
    if (sym_table == rootSymbolTablePtr)
    {

      printf("_________________\n");
      printf("PRINTING GLOBAL SCOPE\n");
      printf("__________________\n");
      is_global = true;
    }
    else
    {
      printf("_________________\n");
      printf("PRINTING SCOPE OF FUNCTION %s\n", sym_table->enclFunName);
      printf("_________________\n");
    }
    type *t1 = NULL;

    if (sym_table->enclFunName)
    {
      t1 = searchHashTablePtr(rootSymbolTablePtr->table, sym_table->enclFunName);
    }

    if (t1 != NULL)
    {
      paramsList *output_param_list = t1->typeInformation.function.outputParList;
      paramsList *input_param_list = t1->typeInformation.function.inputParList;
      parListNode *input_node = NULL;
      if (input_param_list)
      {
        input_node = input_param_list->first;
      }
      parListNode *output_node = NULL;
      if (output_param_list)
      {
        output_node = output_param_list->first;
      }

      while (input_node)
      {
        type *t = input_node->t;
        char *name = input_node->paramName;

        if (t->name == TK_INT)
        {
          printf("%-15s %-15s --- %-15s %-3d --- %-3d INPUT_PARAMETER\n", name, t->enclFunName, keyToToken[t->name], t->width, t->offset);
        }
        else if (t->name == TK_REAL)
        {
          printf("%-15s %-15s --- %-15s %-3d --- %-3d INPUT_PARAMETER\n", name, t->enclFunName, keyToToken[t->name], t->width, t->offset);
        }
        else if (t->name == TK_RECORD)
        {
          printf("%-15s %-15s %s: ", name, sym_table->enclFunName!=NULL?sym_table->enclFunName:t->enclFunName, t->typeInformation.Record.recName);
          printAlias(t);
          printf("\t");
          printTypeExpression(t);
          printf("%-3d    ---   %-3d", t->width, t->offset);
          printf(" INPUT_PARAMETER\n");
        }
        input_node = input_node->next;
      }

      while (output_node)
      {
        type *t = output_node->t;
        char *name = output_node->paramName;
        if (t->name == TK_INT)
        {
          printf("%-15s %-15s --- %-15s %-3d --- %-3d OUTPUT_PARAMETER\n", name, t->enclFunName, keyToToken[t->name], t->width, t->offset);
        }
        else if (t->name == TK_REAL)
        {
          printf("%-15s %-15s --- %-15s %-3d --- %-3d OUTPUT_PARAMETER\n", name, t->enclFunName, keyToToken[t->name], t->width, t->offset);
        }
        else if (t->name == TK_RECORD)
        {
          printf("%-15s %-15s %s: ", name, sym_table->enclFunName!=NULL?sym_table->enclFunName:t->enclFunName, t->typeInformation.Record.recName);
          printAlias(t);
          printf("\t");
          printTypeExpression(t);
          printf("%-3d --- %-3d", t->width, t->offset);
          printf(" OUTPUT_PARAMETER\n");
        }
        output_node = output_node->next;
      }
    }

    for (int i = 0; i < HASHTABLE_SIZE; i++)
    {
      hash_entry entry = sym_table->table[i];

      if (entry.present == true)
      {
        type *t = entry.value;
        char *ID = entry.lexeme;

        if (ID != NULL && (ID[0] != '#' && ID[0] != '_'))
        { 
          char *isGlobal = "---";
          char *g = "isGlobal";
          if (is_global)
          {
            isGlobal = g;
          }

          if (t->name == TK_INT)
          {
            printf("%-15s %-15s --- %-15s %-3d %-15s %-3d ", ID, t->enclFunName, keyToToken[t->name], t->width, isGlobal, t->offset);
          }
          else if (t->name == TK_REAL)
          {
            printf("%-15s %-15s --- %-15s %-3d %-15s %-3d ", ID, t->enclFunName, keyToToken[t->name], t->width, isGlobal, t->offset);
          }
          else if (t->name == TK_RECORD)
          {
            printf("%-15s %-15s %s: ", ID, sym_table->enclFunName!=NULL?sym_table->enclFunName:t->enclFunName, t->typeInformation.Record.recName);
            printAlias(t);
            printf("\t");
            printTypeExpression(t);
            printf("\t");
            printf("%-3d %-15s %-3d ", t->width, isGlobal, t->offset);
          }


          if(t->name!=ALIAS){
            if (is_global)
            {
              printf("---\n");
            }
            else
            {
              printf("LOCAL\n");
            }

          }
        }
      }
    }

    printSymbolTable(sym_table->leftmostChildSymTab);

    SymbolTable *temp = sym_table->brotherSymTab;
      if(temp){
      printSymbolTable(temp);

      }
  }