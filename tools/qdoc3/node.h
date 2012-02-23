/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/
**
** This file is part of the tools applications of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this
** file. Please review the following information to ensure the GNU Lesser
** General Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights. These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU General
** Public License version 3.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of this
** file. Please review the following information to ensure the GNU General
** Public License version 3.0 requirements will be met:
** http://www.gnu.org/copyleft/gpl.html.
**
** Other Usage
** Alternatively, this file may be used in accordance with the terms and
** conditions contained in a signed written agreement between you and Nokia.
**
**
**
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

/*
  node.h
*/

#ifndef NODE_H
#define NODE_H

#include <qdir.h>
#include <qmap.h>
#include <qpair.h>
#include <qstringlist.h>

#include "codechunk.h"
#include "doc.h"
#include "location.h"
#include "text.h"

QT_BEGIN_NAMESPACE

class Node;
class ClassNode;
class InnerNode;
class ClassNode;
class ExampleNode;
class QmlClassNode;
class Tree;

typedef QMap<QString, const Node*> NodeMap;
typedef QMultiMap<QString, Node*> NodeMultiMap;
typedef QMultiMap<QString, const ExampleNode*> ExampleNodeMap;
typedef QList<QPair<QString,QString> > ImportList;

class Node
{
 public:
    enum Type { 
        Namespace, 
        Class, 
        Fake, 
        Enum, 
        Typedef, 
        Function, 
        Property,
        Variable, 
        Target,
        QmlProperty,
        QmlSignal,
        QmlSignalHandler,
        QmlMethod,
        LastType
    };

    enum SubType { 
        NoSubType,
        Example,
        HeaderFile,
        File,
        Image,
        Group,
        Module,
        Page,
        ExternalPage,
        QmlClass,
        QmlPropertyGroup,
        QmlBasicType,
        QmlModule,
        DitaMap,
        Collision,
        LastSubtype
    };
    
    enum Access { Public, Protected, Private };
    
    enum Status { 
        Compat, 
        Obsolete, 
        Deprecated, 
        Preliminary, 
        Commendable, 
        Main, 
        Internal 
    }; // don't reorder this enum
    
    enum ThreadSafeness { 
        UnspecifiedSafeness, 
        NonReentrant, 
        Reentrant, 
        ThreadSafe 
    };
    
    enum LinkType { 
        StartLink, 
        NextLink, 
        PreviousLink,
        ContentsLink, 
        IndexLink,
        InheritsLink /*, 
        GlossaryLink, 
        CopyrightLink,
        ChapterLink, 
        SectionLink, 
        SubsectionLink, 
        AppendixLink */ 
    };

    enum PageType {
        NoPageType,
        ApiPage,
        ArticlePage,
        ExamplePage,
        HowToPage,
        OverviewPage,
        TutorialPage,
        FAQPage,
        DitaMapPage,
        OnBeyondZebra
    };

    virtual ~Node();

    void setAccess(Access access) { access_ = access; }
    void setLocation(const Location& location) { loc = location; }
    void setDoc(const Doc& doc, bool replace = false);
    void setStatus(Status status) { status_ = status; }
    void setThreadSafeness(ThreadSafeness safeness) { safeness_ = safeness; }
    void setSince(const QString &since);
    void setRelates(InnerNode* pseudoParent);
    void setModuleName(const QString &module) { mod = module; }
    void setLink(LinkType linkType, const QString &link, const QString &desc);
    void setUrl(const QString &url);
    void setTemplateStuff(const QString &templateStuff) { templateStuff_ = templateStuff; }
    void setPageType(PageType t) { pageType_ = t; }
    void setPageType(const QString& t);
    void setParent(InnerNode* n) { parent_ = n; }

    virtual bool isInnerNode() const = 0;
    virtual bool isReimp() const { return false; }
    virtual bool isFunction() const { return false; }
    virtual bool isQmlNode() const { return false; }
    virtual bool isInternal() const { return false; }
    virtual bool isQtQuickNode() const { return false; }
    virtual bool isAbstract() const { return false; }
    virtual void setAbstract(bool ) { }
    virtual QString title() const { return QString(); }
    Type type() const { return nodeType_; }
    virtual SubType subType() const { return NoSubType; }
    InnerNode* parent() const { return parent_; }
    InnerNode* relates() const { return relatesTo_; }
    const QString& name() const { return name_; }
    QMap<LinkType, QPair<QString,QString> > links() const { return linkMap; }
    QString moduleName() const;
    QString url() const;
    virtual QString nameForLists() const { return name_; }

    Access access() const { return access_; }
    QString accessString() const;
    const Location& location() const { return loc; }
    const Doc& doc() const { return d; }
    Status status() const { return status_; }
    Status inheritedStatus() const;
    ThreadSafeness threadSafeness() const;
    ThreadSafeness inheritedThreadSafeness() const;
    QString since() const { return sinc; }
    QString templateStuff() const { return templateStuff_; }
    PageType pageType() const { return pageType_; }
    QString pageTypeString() const;
    QString nodeTypeString() const;
    QString nodeSubtypeString() const;
    virtual void addPageKeywords(const QString& ) { }

    void clearRelated() { relatesTo_ = 0; }

    virtual QString fileBase() const;
    QString guid() const;
    QString ditaXmlHref();
    QString extractClassName(const QString &string) const;
    virtual QString qmlModuleName() const { return qmlModuleName_; }
    virtual QString qmlModuleVersion() const { return qmlModuleVersion_; }
    virtual QString qmlModuleIdentifier() const { return qmlModuleName_ + qmlModuleVersion_; }
    virtual void setQmlModuleName(const QString& );
    virtual const ClassNode* classNode() const { return 0; }
    virtual void clearCurrentChild() { }
    virtual const ImportList* importList() const { return 0; }
    virtual void setImportList(const ImportList& ) { }
    virtual const Node* applyModuleIdentifier(const Node* ) const { return 0; }
    const QmlClassNode* qmlClassNode() const;
    const ClassNode* declarativeCppNode() const;
    const QString& outputSubdirectory() const { return outSubDir_; }
    QString fullDocumentName() const;
    static QString cleanId(QString str);
    QString idForNode() const;

    static QString pageTypeString(unsigned t);
    static QString nodeTypeString(unsigned t);
    static QString nodeSubtypeString(unsigned t);

 protected:
    Node(Type type, InnerNode* parent, const QString& name);

 private:

#ifdef Q_WS_WIN
    Type nodeType_;
    Access access_;
    ThreadSafeness safeness_;
    PageType pageType_;
    Status status_;
#else
    Type nodeType_ : 4;
    Access access_ : 2;
    ThreadSafeness safeness_ : 2;
    PageType pageType_ : 4;
    Status status_ : 3;
#endif
    InnerNode* parent_;
    InnerNode* relatesTo_;
    QString name_;
    Location loc;
    Doc d;
    QMap<LinkType, QPair<QString, QString> > linkMap;
    QString mod;
    QString url_;
    QString sinc;
    QString templateStuff_;
    mutable QString uuid;
    QString outSubDir_;
    QString qmlModuleName_;
    QString qmlModuleVersion_;
    static QStringMap operators_;
};

class FunctionNode;
class EnumNode;
class NameCollisionNode;

typedef QList<Node*> NodeList;
typedef QMap<QString, const Node*> NodeMap;
typedef QMultiMap<QString, Node*> NodeMultiMap;

class InnerNode : public Node
{
 public:
    virtual ~InnerNode();

    Node* findNode(const QString& name);
    Node* findNode(const QString& name, bool qml);
    Node* findNode(const QString& name, Type type);
    void findNodes(const QString& name, QList<Node*>& n);
    FunctionNode* findFunctionNode(const QString& name);
    FunctionNode* findFunctionNode(const FunctionNode* clone);
    void addInclude(const QString &include);
    void setIncludes(const QStringList &includes);
    void setOverload(const FunctionNode* func, bool overlode);
    void normalizeOverloads();
    void makeUndocumentedChildrenInternal();
    void clearCurrentChildPointers();
    void deleteChildren();
    void removeFromRelated();

    virtual bool isInnerNode() const { return true; }
    const Node* findNode(const QString& name) const;
    const Node* findNode(const QString& name, bool qml) const;
    const Node* findNode(const QString& name, Type type) const;
    const FunctionNode* findFunctionNode(const QString& name) const;
    const FunctionNode* findFunctionNode(const FunctionNode* clone) const;
    const EnumNode* findEnumNodeForValue(const QString &enumValue) const;
    const NodeList & childNodes() const { return children; }
    const NodeList & relatedNodes() const { return related_; }
    int count() const { return children.size(); }
    int overloadNumber(const FunctionNode* func) const;
    int numOverloads(const QString& funcName) const;
    NodeList overloads(const QString &funcName) const;
    const QStringList& includes() const { return inc; }

    QStringList primaryKeys();
    QStringList secondaryKeys();
    const QStringList& pageKeywords() const { return pageKeywds; }
    virtual void addPageKeywords(const QString& t) { pageKeywds << t; }
    virtual void setCurrentChild() { }
    virtual void setCurrentChild(InnerNode* ) { }

 protected:
    InnerNode(Type type, InnerNode* parent, const QString& name);

 private:
    friend class Node;
    friend class NameCollisionNode;

    static bool isSameSignature(const FunctionNode* f1, const FunctionNode* f2);
    void addChild(Node* child);
    void removeRelated(Node* pseudoChild);
    void removeChild(Node* child);

    QStringList pageKeywds;
    QStringList inc;
    NodeList children;
    NodeList enumChildren;
    NodeList related_;
    QMap<QString, Node*> childMap;
    QMap<QString, Node*> primaryFunctionMap;
    QMap<QString, NodeList> secondaryFunctionMap;
};

class LeafNode : public Node
{
 public:
    LeafNode();
    virtual ~LeafNode() { }

    virtual bool isInnerNode() const;

 protected:
    LeafNode(Type type, InnerNode* parent, const QString& name);
    LeafNode(InnerNode* parent, Type type, const QString& name);
};

class NamespaceNode : public InnerNode
{
 public:
    NamespaceNode(InnerNode* parent, const QString& name);
    virtual ~NamespaceNode() { }
};

class ClassNode;

struct RelatedClass
{
    RelatedClass() { }
    RelatedClass(Node::Access access0, 
                 ClassNode* node0,
                 const QString& dataTypeWithTemplateArgs0 = "")
      : access(access0), 
        node(node0),
        dataTypeWithTemplateArgs(dataTypeWithTemplateArgs0) { }
    QString accessString() const;

    Node::Access        access;
    ClassNode*          node;
    QString             dataTypeWithTemplateArgs;
};

class PropertyNode;

class ClassNode : public InnerNode
{
 public:
    ClassNode(InnerNode* parent, const QString& name);
    virtual ~ClassNode() { }

    void addBaseClass(Access access, 
                      ClassNode* node, 
                      const QString &dataTypeWithTemplateArgs = "");
    void fixBaseClasses();

    const QList<RelatedClass> &baseClasses() const { return bases; }
    const QList<RelatedClass> &derivedClasses() const { return derived; }
    const QList<RelatedClass> &ignoredBaseClasses() const { return ignoredBases; }

    bool hideFromMainList() const { return hidden; }
    void setHideFromMainList(bool value) { hidden = value; }

    QString serviceName() const { return sname; }
    void setServiceName(const QString& value) { sname = value; }
    const QmlClassNode* qmlElement() const { return qmlelement; }
    void setQmlElement(QmlClassNode* qcn) { qmlelement = qcn; }
    virtual bool isAbstract() const { return abstract; }
    virtual void setAbstract(bool b) { abstract = b; }
    const PropertyNode* findPropertyNode(const QString& name) const;
    const QmlClassNode* findQmlBaseNode() const;

 private:
    QList<RelatedClass> bases;
    QList<RelatedClass> derived;
    QList<RelatedClass> ignoredBases;
    bool hidden;
    bool abstract;
    QString sname;
    QmlClassNode* qmlelement;
};

class FakeNode : public InnerNode
{
 public:

    FakeNode(InnerNode* parent,
             const QString& name,
             SubType subType,
             PageType ptype);
    virtual ~FakeNode() { }

    void setTitle(const QString &title) { title_ = title; }
    void setSubTitle(const QString &subTitle) { subtitle_ = subTitle; }
    void addGroupMember(Node* node) { nodeList.append(node); }
    void addQmlModuleMember(Node* node) { nodeList.append(node); }

    SubType subType() const { return nodeSubtype_; }
    virtual QString title() const;
    virtual QString fullTitle() const;
    virtual QString subTitle() const;
    virtual QString imageFileName() const { return QString(); }
    const NodeList& groupMembers() const { return nodeList; }
    const NodeList& qmlModuleMembers() const { return nodeList; }
    virtual QString nameForLists() const { return title(); }
    virtual void setImageFileName(const QString& ) { }

  protected:
    SubType nodeSubtype_;
    QString title_;
    QString subtitle_;
    NodeList nodeList; // used for groups and QML modules.
};

class NameCollisionNode : public FakeNode
{
 public:
    NameCollisionNode(InnerNode* child);
    ~NameCollisionNode();
    const InnerNode* currentChild() const { return current; }
    virtual void setCurrentChild(InnerNode* child) { current = child; }
    virtual void clearCurrentChild() { current = 0; }
    virtual bool isQmlNode() const;
    virtual const Node* applyModuleIdentifier(const Node* origin) const;
    const InnerNode* findAny(Node::Type t, Node::SubType st) const;
    void addCollision(InnerNode* child);
    const QMap<QString,QString>& linkTargets() const { return targets; }
    void addLinkTarget(const QString& t, const QString& v) { targets.insert(t,v); }

 private:
    InnerNode* current;
    QMap<QString,QString> targets;
};

class ExampleNode : public FakeNode
{
 public:
    ExampleNode(InnerNode* parent, const QString& name);
    virtual ~ExampleNode() { }
    virtual QString imageFileName() const { return imageFileName_; }
    virtual void setImageFileName(const QString& ifn) { imageFileName_ = ifn; }

    static void terminate() { exampleNodeMap.clear(); }

 public:
    static ExampleNodeMap exampleNodeMap;

 private:
    QString imageFileName_;
};

class QmlClassNode : public FakeNode
{
 public:
    QmlClassNode(InnerNode* parent, 
                 const QString& name, 
                 const ClassNode* cn);
    virtual ~QmlClassNode();
    virtual bool isQmlNode() const { return true; }
    virtual bool isQtQuickNode() const { return (qmlModuleName() == "QtQuick"); }
    virtual const ClassNode* classNode() const { return cnode; }
    virtual QString fileBase() const;
    virtual void setCurrentChild();
    virtual void clearCurrentChild();
    virtual const ImportList* importList() const { return &importList_; }
    virtual void setImportList(const ImportList& il) { importList_ = il; }
    virtual bool isAbstract() const { return abstract; }
    virtual void setAbstract(bool b) { abstract = b; }
    const FakeNode* qmlBase() const { return base_; }
    void resolveInheritance(const Tree* tree);
    static void addInheritedBy(const QString& base, Node* sub);
    static void subclasses(const QString& base, NodeList& subs);
    static void terminate();

 public:
    static bool qmlOnly;
    static QMultiMap<QString,Node*> inheritedBy;
    static QMap<QString, QmlClassNode*> moduleMap;

 private:
    bool abstract;
    const ClassNode*    cnode;
    const FakeNode*     base_;
    ImportList          importList_;
};

class QmlBasicTypeNode : public FakeNode
{
 public:
    QmlBasicTypeNode(InnerNode* parent, 
                     const QString& name);
    virtual ~QmlBasicTypeNode() { }
    virtual bool isQmlNode() const { return true; }
};

class QmlPropGroupNode : public FakeNode
{
 public:
    QmlPropGroupNode(QmlClassNode* parent, 
                     const QString& name,
                     bool attached);
    virtual ~QmlPropGroupNode() { }
    virtual bool isQmlNode() const { return true; }
    virtual bool isQtQuickNode() const { return parent()->isQtQuickNode(); }
    virtual QString qmlModuleName() const { return parent()->qmlModuleName(); }
    virtual QString qmlModuleVersion() const { return parent()->qmlModuleVersion(); }
    virtual QString qmlModuleIdentifier() const { return parent()->qmlModuleIdentifier(); }

    const QString& element() const { return parent()->name(); }
    void setDefault() { isdefault_ = true; }
    void setReadOnly(int ro) { readOnly_ = ro; }
    int getReadOnly() const { return readOnly_; }
    bool isDefault() const { return isdefault_; }
    bool isAttached() const { return attached_; }
    bool isReadOnly() const { return (readOnly_ > 0); }

 private:
    bool    isdefault_;
    bool    attached_;
    int     readOnly_;
};

class QmlPropertyNode;

class QmlPropertyNode : public LeafNode
{
 public:
    QmlPropertyNode(QmlClassNode *parent,
                    const QString& name,
                    const QString& type,
                    bool attached);
    QmlPropertyNode(QmlPropGroupNode* parent, 
                    const QString& name,
                    const QString& type,
                    bool attached);
    QmlPropertyNode(QmlPropertyNode* parent,
                    const QString& name,
                    const QString& type,
                    bool attached);
    virtual ~QmlPropertyNode() { }

    void setDataType(const QString& dataType) { type_ = dataType; }
    void setStored(bool stored) { sto = toTrool(stored); }
    void setDesignable(bool designable) { des = toTrool(designable); }
    void setWritable(bool writable) { wri = toTrool(writable); }

    const QString &dataType() const { return type_; }
    QString qualifiedDataType() const { return type_; }
    void setDefault() { isdefault_ = true; }
    void setReadOnly(int ro) { readOnly_ = ro; }
    int getReadOnly() const { return readOnly_; }
    bool isDefault() const { return isdefault_; }
    bool isStored() const { return fromTrool(sto,true); }
    bool isDesignable() const { return fromTrool(des,false); }
    bool isWritable(const Tree* tree) const;
    bool isAttached() const { return attached_; }
    bool isReadOnly() const { return (readOnly_ > 0); }
    virtual bool isQmlNode() const { return true; }
    virtual bool isQtQuickNode() const { return parent()->isQtQuickNode(); }
    virtual QString qmlModuleName() const { return parent()->qmlModuleName(); }
    virtual QString qmlModuleVersion() const { return parent()->qmlModuleVersion(); }
    virtual QString qmlModuleIdentifier() const { return parent()->qmlModuleIdentifier(); }

    const PropertyNode *correspondingProperty(const Tree *tree) const;

    const QString& element() const { return static_cast<QmlPropGroupNode*>(parent())->element(); }
    void appendQmlPropNode(QmlPropertyNode* p) { qmlPropNodes_.append(p); }
    const NodeList& qmlPropNodes() const { return qmlPropNodes_; }

 private:
    enum Trool { Trool_True, Trool_False, Trool_Default };

    static Trool toTrool(bool boolean);
    static bool fromTrool(Trool troolean, bool defaultValue);

    QString type_;
    Trool   sto;
    Trool   des;
    Trool   wri;
    bool    isdefault_;
    bool    attached_;
    int     readOnly_;
    NodeList qmlPropNodes_;
};

class EnumItem
{
 public:
    EnumItem() { }
    EnumItem(const QString& name, const QString& value)
	: nam(name), val(value) { }
    EnumItem(const QString& name, const QString& value, const Text &txt)
	: nam(name), val(value), txt(txt) { }

    const QString& name() const { return nam; }
    const QString& value() const { return val; }
    const Text &text() const { return txt; }

 private:
    QString nam;
    QString val;
    Text txt;
};

class TypedefNode;

class EnumNode : public LeafNode
{
 public:
    EnumNode(InnerNode* parent, const QString& name);
    virtual ~EnumNode() { }

    void addItem(const EnumItem& item);
    void setFlagsType(TypedefNode* typedeff);
    bool hasItem(const QString &name) const { return names.contains(name); }

    const QList<EnumItem>& items() const { return itms; }
    Access itemAccess(const QString& name) const;
    const TypedefNode* flagsType() const { return ft; }
    QString itemValue(const QString &name) const;

 private:
    QList<EnumItem> itms;
    QSet<QString> names;
    const TypedefNode* ft;
};

class TypedefNode : public LeafNode
{
 public:
    TypedefNode(InnerNode* parent, const QString& name);
    virtual ~TypedefNode() { }

    const EnumNode* associatedEnum() const { return ae; }

 private:
    void setAssociatedEnum(const EnumNode* enume);

    friend class EnumNode;

    const EnumNode* ae;
};

inline void EnumNode::setFlagsType(TypedefNode* typedeff)
{
    ft = typedeff;
    typedeff->setAssociatedEnum(this);
}


class Parameter
{
 public:
    Parameter() {}
    Parameter(const QString& leftType, 
              const QString& rightType = "",
              const QString& name = "", 
              const QString& defaultValue = "");
    Parameter(const Parameter& p);

    Parameter& operator=(const Parameter& p);

    void setName(const QString& name) { nam = name; }

    bool hasType() const { return lef.length() + rig.length() > 0; }
    const QString& leftType() const { return lef; }
    const QString& rightType() const { return rig; }
    const QString& name() const { return nam; }
    const QString& defaultValue() const { return def; }

    QString reconstruct(bool value = false) const;

 private:
    QString lef;
    QString rig;
    QString nam;
    QString def;
};

class PropertyNode;

class FunctionNode : public LeafNode
{
 public:
    enum Metaness { 
        Plain, 
        Signal, 
        Slot,
        Ctor,
        Dtor, 
        MacroWithParams,
        MacroWithoutParams, 
        Native };
    enum Virtualness { NonVirtual, ImpureVirtual, PureVirtual };

    FunctionNode(InnerNode* parent, const QString &name);
    FunctionNode(Type type, InnerNode* parent, const QString &name, bool attached);
    virtual ~FunctionNode() { }

    void setReturnType(const QString& returnType) { rt = returnType; }
    void setParentPath(const QStringList& parentPath) { pp = parentPath; }
    void setMetaness(Metaness metaness) { met = metaness; }
    void setVirtualness(Virtualness virtualness);
    void setConst(bool conste) { con = conste; }
    void setStatic(bool statique) { sta = statique; }
    void setOverload(bool overlode);
    void setReimp(bool r);
    void addParameter(const Parameter& parameter);
    inline void setParameters(const QList<Parameter>& parameters);
    void borrowParameterNames(const FunctionNode* source);
    void setReimplementedFrom(FunctionNode* from);

    const QString& returnType() const { return rt; }
    Metaness metaness() const { return met; }
    bool isMacro() const { 
        return met == MacroWithParams || met == MacroWithoutParams; 
    }
    Virtualness virtualness() const { return vir; }
    bool isConst() const { return con; }
    bool isStatic() const { return sta; }
    bool isOverload() const { return ove; }
    bool isReimp() const { return reimp; }
    bool isFunction() const { return true; }
    int overloadNumber() const;
    int numOverloads() const;
    const QList<Parameter>& parameters() const { return params; }
    QStringList parameterNames() const;
    QString rawParameters(bool names = false, bool values = false) const;
    const FunctionNode* reimplementedFrom() const { return rf; }
    const QList<FunctionNode*> &reimplementedBy() const { return rb; }
    const PropertyNode* associatedProperty() const { return ap; }
    const QStringList& parentPath() const { return pp; }

    QStringList reconstructParams(bool values = false) const;
    QString signature(bool values = false) const;
    const QString& element() const { return parent()->name(); }
    bool isAttached() const { return attached_; }
    virtual bool isInternal() const;
    virtual bool isQmlNode() const { 
        return ((type() == QmlSignal) ||
                (type() == QmlMethod) ||
                (type() == QmlSignalHandler));
    }
    virtual bool isQtQuickNode() const { return parent()->isQtQuickNode(); }
    virtual QString qmlModuleName() const { return parent()->qmlModuleName(); }
    virtual QString qmlModuleVersion() const { return parent()->qmlModuleVersion(); }
    virtual QString qmlModuleIdentifier() const { return parent()->qmlModuleIdentifier(); }

    void debug() const;

 private:
    void setAssociatedProperty(PropertyNode* property);

    friend class InnerNode;
    friend class PropertyNode;

    QString     rt;
    QStringList pp;
#ifdef Q_WS_WIN
    Metaness    met;
    Virtualness vir;
#else
    Metaness met : 4;
    Virtualness vir : 2;
#endif
    bool con : 1;
    bool sta : 1;
    bool ove : 1;
    bool reimp: 1; 
    bool attached_: 1;
    QList<Parameter> params;
    const FunctionNode* rf;
    const PropertyNode* ap;
    QList<FunctionNode*> rb;
};

class PropertyNode : public LeafNode
{
 public:
    enum FunctionRole { Getter, Setter, Resetter, Notifier };
    enum { NumFunctionRoles = Notifier + 1 };

    PropertyNode(InnerNode* parent, const QString& name);
    virtual ~PropertyNode() { }

    void setDataType(const QString& dataType) { type_ = dataType; }
    void addFunction(FunctionNode* function, FunctionRole role);
    void addSignal(FunctionNode* function, FunctionRole role);
    void setStored(bool stored) { sto = toTrool(stored); }
    void setDesignable(bool designable) { des = toTrool(designable); }
    void setScriptable(bool scriptable) { scr = toTrool(scriptable); }
    void setWritable(bool writable) { wri = toTrool(writable); }
    void setUser(bool user) { usr = toTrool(user); }
    void setOverriddenFrom(const PropertyNode* baseProperty);
    void setRuntimeDesFunc(const QString& rdf) { runtimeDesFunc = rdf; }
    void setRuntimeScrFunc(const QString& scrf) { runtimeScrFunc = scrf; }
    void setConstant() { cst = true; }
    void setFinal() { fnl = true; }
    void setRevision(int revision) { rev = revision; }

    const QString &dataType() const { return type_; }
    QString qualifiedDataType() const;
    NodeList functions() const;
    NodeList functions(FunctionRole role) const { return funcs[(int)role]; }
    NodeList getters() const { return functions(Getter); }
    NodeList setters() const { return functions(Setter); }
    NodeList resetters() const { return functions(Resetter); }
    NodeList notifiers() const { return functions(Notifier); }
    bool isStored() const { return fromTrool(sto, storedDefault()); }
    bool isDesignable() const { return fromTrool(des, designableDefault()); }
    bool isScriptable() const { return fromTrool(scr, scriptableDefault()); }
    const QString& runtimeDesignabilityFunction() const { return runtimeDesFunc; }
    const QString& runtimeScriptabilityFunction() const { return runtimeScrFunc; }
    bool isWritable() const { return fromTrool(wri, writableDefault()); }
    bool isUser() const { return fromTrool(usr, userDefault()); }
    bool isConstant() const { return cst; }
    bool isFinal() const { return fnl; }
    const PropertyNode* overriddenFrom() const { return overrides; }

    bool storedDefault() const { return true; }
    bool userDefault() const { return false; }
    bool designableDefault() const { return !setters().isEmpty(); }
    bool scriptableDefault() const { return true; }
    bool writableDefault() const { return !setters().isEmpty(); }

 private:
    enum Trool { Trool_True, Trool_False, Trool_Default };

    static Trool toTrool(bool boolean);
    static bool fromTrool(Trool troolean, bool defaultValue);

    QString type_;
    QString runtimeDesFunc;
    QString runtimeScrFunc;
    NodeList funcs[NumFunctionRoles];
    Trool sto;
    Trool des;
    Trool scr;
    Trool wri;
    Trool usr;
    bool cst;
    bool fnl;
    int rev;
    const PropertyNode* overrides;
};

inline void FunctionNode::setParameters(const QList<Parameter> &parameters)
{
    params = parameters;
}

inline void PropertyNode::addFunction(FunctionNode* function, FunctionRole role)
{
    funcs[(int)role].append(function);
    function->setAssociatedProperty(this);
}

inline void PropertyNode::addSignal(FunctionNode* function, FunctionRole role)
{
    funcs[(int)role].append(function);
}

inline NodeList PropertyNode::functions() const
{
    NodeList list;
    for (int i = 0; i < NumFunctionRoles; ++i)
	list += funcs[i];
    return list;
}

class VariableNode : public LeafNode
{
 public:
    VariableNode(InnerNode* parent, const QString &name);
    virtual ~VariableNode() { }

    void setLeftType(const QString &leftType) { lt = leftType; }
    void setRightType(const QString &rightType) { rt = rightType; }
    void setStatic(bool statique) { sta = statique; }

    const QString &leftType() const { return lt; }
    const QString &rightType() const { return rt; }
    QString dataType() const { return lt + rt; }
    bool isStatic() const { return sta; }

 private:
    QString lt;
    QString rt;
    bool sta;
};

inline VariableNode::VariableNode(InnerNode* parent, const QString &name)
    : LeafNode(Variable, parent, name), sta(false)
{
    // nothing.
}

class TargetNode : public LeafNode
{
 public:
    TargetNode(InnerNode* parent, const QString& name);
    virtual ~TargetNode() { }

    virtual bool isInnerNode() const;
};

class DitaMapNode : public FakeNode
{
 public:
    DitaMapNode(InnerNode* parent, const QString& name)
        : FakeNode(parent, name, Node::Page, Node::DitaMapPage) { }
    virtual ~DitaMapNode() { }

    const QList<Topicref*>& map() const { return doc().ditamap(); }
};

QT_END_NAMESPACE

#endif
