#include "DbViewerPanel.h"
#include "DbSettingDialog.h"
#include "SqlCommandPanel.h"
#include "ErdPanel.h"
#include "AdapterSelectDlg.h"
#include "window_locker.h"
#include "globals.h"

#include <wx/wfstream.h>
#include <wx/imaglist.h>

#define wxID_CONNECT          15534
#define wxID_CLOSE_CONNECTION 15535
#define wxID_TOOL_REFRESH     15536
#define wxID_TOOL_ERD         15537
#define wxID_TOOL_PREVIEW     15538

DbViewerPanel::DbViewerPanel(wxWindow *parent, wxWindow* notebook, IManager* pManager)
	: _DbViewerPanel(parent)
{
	m_pNotebook = notebook;
	m_pGlobalParent = parent;
	m_mgr = pManager;

	MSWSetNativeTheme(m_treeDatabases);

	m_pDbAdapter = NULL;
	m_pConnections = new xsSerializable();

	m_pThumbnail = new wxSFThumbnail(this);
	m_pThumbnail->SetSizeHints(wxSize(-1, 200));

	GetSizer()->Add(m_pThumbnail, 0, wxEXPAND);
	GetSizer()->Hide(m_pThumbnail);

	GetSizer()->Layout();

	// replace the icons...
	BitmapLoader *bmpLoader = pManager->GetStdIcons();
	
	m_toolBar1->AddTool( wxID_CONNECT, _("Open connection"),         bmpLoader->LoadBitmap(wxT("db-explorer/16/connect")),          wxNullBitmap, wxITEM_NORMAL, _("Open new connection"), _("Open new connection"), NULL );
	m_toolBar1->AddTool( wxID_CLOSE_CONNECTION, _("tool"),           bmpLoader->LoadBitmap(wxT("db-explorer/16/disconnect")),       wxNullBitmap, wxITEM_NORMAL, _("Close selected connection"), _("Close selected connection"), NULL );
	m_toolBar1->AddTool( wxID_TOOL_REFRESH, _("tool"),               bmpLoader->LoadBitmap(wxT("db-explorer/16/database_refresh")), wxNullBitmap, wxITEM_NORMAL, _("Refresh View"), wxEmptyString, NULL );
	m_toolBar1->AddTool( wxID_TOOL_ERD, _("ERD"),                    bmpLoader->LoadBitmap(wxT("db-explorer/16/table")),            wxNullBitmap, wxITEM_NORMAL, _("Open ERD View"), wxEmptyString, NULL );
	m_toolBar1->AddTool( wxID_TOOL_PREVIEW, _("Show ERD Thumbnail"), bmpLoader->LoadBitmap(wxT("db-explorer/16/thumbnail")), _("Show ERD Thumbnail"), wxITEM_CHECK);
	m_toolBar1->Realize();

	Layout();

	m_mgr->GetTheApp()->Connect(wxEVT_COMMAND_BOOK_PAGE_CHANGED,NotebookEventHandler(DbViewerPanel::OnPageChange), NULL, this);
	m_mgr->GetTheApp()->Connect(wxEVT_COMMAND_BOOK_PAGE_CLOSING,NotebookEventHandler(DbViewerPanel::OnPageClose), NULL, this);

	this->Connect( wxID_CONNECT, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( DbViewerPanel::OnConncectClick ) );
	this->Connect( wxID_CONNECT, wxEVT_UPDATE_UI, wxUpdateUIEventHandler( DbViewerPanel::OnConncectUI ) );
	this->Connect( wxID_CLOSE_CONNECTION, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( DbViewerPanel::OnToolCloseClick ) );
	this->Connect( wxID_CLOSE_CONNECTION, wxEVT_UPDATE_UI, wxUpdateUIEventHandler( DbViewerPanel::OnToolCloseUI ) );
	this->Connect( wxID_TOOL_REFRESH, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( DbViewerPanel::OnRefreshClick ) );
	this->Connect( wxID_TOOL_REFRESH, wxEVT_UPDATE_UI, wxUpdateUIEventHandler( DbViewerPanel::OnToolCloseUI ) );
	this->Connect( wxID_TOOL_ERD, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( DbViewerPanel::OnERDClick ) );
	this->Connect( wxID_TOOL_PREVIEW, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( DbViewerPanel::OnShowThumbnail ) );
}

DbViewerPanel::~DbViewerPanel()
{
	m_mgr->GetTheApp()->Disconnect(wxEVT_COMMAND_BOOK_PAGE_CHANGED,NotebookEventHandler(DbViewerPanel::OnPageChange), NULL, this);
	m_mgr->GetTheApp()->Disconnect(wxEVT_COMMAND_BOOK_PAGE_CLOSING,NotebookEventHandler(DbViewerPanel::OnPageClose), NULL, this);
	delete m_pDbAdapter;

	this->Disconnect( wxID_CONNECT, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( DbViewerPanel::OnConncectClick ) );
	this->Disconnect( wxID_CONNECT, wxEVT_UPDATE_UI, wxUpdateUIEventHandler( DbViewerPanel::OnConncectUI ) );
	this->Disconnect( wxID_CLOSE_CONNECTION, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( DbViewerPanel::OnToolCloseClick ) );
	this->Disconnect( wxID_CLOSE_CONNECTION, wxEVT_UPDATE_UI, wxUpdateUIEventHandler( DbViewerPanel::OnToolCloseUI ) );
	this->Disconnect( wxID_TOOL_REFRESH, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( DbViewerPanel::OnRefreshClick ) );
	this->Disconnect( wxID_TOOL_REFRESH, wxEVT_UPDATE_UI, wxUpdateUIEventHandler( DbViewerPanel::OnToolCloseUI ) );
	this->Disconnect( wxID_TOOL_ERD, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( DbViewerPanel::OnERDClick ) );
}

void DbViewerPanel::OnConncectClick(wxCommandEvent& event)
{
	DbSettingDialog dlg(this, m_mgr->GetTheApp()->GetTopWindow());
	dlg.ShowModal();
	RefreshDbView();
}

void DbViewerPanel::OnConncectUI(wxUpdateUIEvent& event)
{
	event.Enable( !m_pConnections->HasChildren() );
}

void DbViewerPanel::OnItemActivate(wxTreeEvent& event)
{
	DbItem* item = (DbItem*) m_treeDatabases->GetItemData(event.GetItem());
	if (item) {
		wxMouseState cState = wxGetMouseState();

		wxString pagename;
		if (DBETable* tab =  wxDynamicCast(item->GetData(), DBETable)) {
			if( cState.ControlDown() ) {
				pagename = CreatePanelName(tab, DbViewerPanel::Erd);
				ErdPanel *erdpanel = new ErdPanel(m_pNotebook,tab->GetDbAdapter()->Clone(),m_pConnections, (DBETable*) tab->Clone() );
				m_mgr->AddEditorPage(erdpanel, pagename);
				m_pagesAdded.Add(pagename);
				erdpanel->SetFocus();

			} else {
#if defined (__WXMSW__)
				clWindowUpdateLocker locker(m_mgr->GetEditorPaneNotebook());
#endif
				pagename = CreatePanelName(tab, DbViewerPanel::Sql);
				if(!DoSelectPage(pagename)) {
					SQLCommandPanel *sqlpage = new SQLCommandPanel(m_pNotebook,tab->GetDbAdapter()->Clone(),tab->GetParentName(),tab->GetName());
					m_mgr->AddEditorPage(sqlpage, pagename);
					sqlpage->SetFocus();
					m_pagesAdded.Add(pagename);
				}
			}
		}

		if (View* pView = wxDynamicCast(item->GetData(), View)) {
			pagename = CreatePanelName(pView, DbViewerPanel::Sql);
			if(!DoSelectPage(pagename)) {
				SQLCommandPanel *sqlpage = new SQLCommandPanel(m_pNotebook,pView->GetDbAdapter()->Clone(),pView->GetParentName(),pView->GetName());
				m_mgr->AddEditorPage(sqlpage, pagename);
				m_pagesAdded.Add(pagename);
				sqlpage->SetFocus();
			}
		}

		if (Database* db = wxDynamicCast(item->GetData(), Database)) {
			if( cState.ControlDown() ) {
				pagename = CreatePanelName(db, DbViewerPanel::Erd);
				ErdPanel *erdpanel = new ErdPanel(m_pNotebook,db->GetDbAdapter()->Clone(),m_pConnections,(Database*)db->Clone());
				m_mgr->AddEditorPage(erdpanel, pagename);
				erdpanel->SetFocus();

			} else {
				pagename = CreatePanelName(db, DbViewerPanel::Sql);
				if(!DoSelectPage(pagename)) {
					SQLCommandPanel *sqlpage = new SQLCommandPanel(m_pNotebook,db->GetDbAdapter()->Clone(),db->GetName(),wxT(""));
#ifndef __WXMSW__
					sqlpage->Show();
#endif
					m_mgr->AddEditorPage(sqlpage, pagename);
					m_pagesAdded.Add(pagename);
					sqlpage->SetFocus();
				}
			}
		}
	}
}

void DbViewerPanel::OnRefreshClick(wxCommandEvent& event)
{
	// Refresh all connections
	wxTreeItemId root = m_treeDatabases->GetRootItem();
	if(root.IsOk()) {
		wxTreeItemIdValue cookie;
		wxTreeItemId child = m_treeDatabases->GetFirstChild(root, cookie);
		while(child.IsOk()) {
			DbItem* data = (DbItem*) m_treeDatabases->GetItemData(child);
			if (data) {
				DbConnection* pCon = wxDynamicCast(data->GetData(), DbConnection);
				if (pCon) pCon->RefreshChildren();
				Database* pDb = wxDynamicCast(data->GetData(),Database);
				if (pDb) pDb->RefreshChildren(false);
				DBETable* pTab = wxDynamicCast(data->GetData(), DBETable);
				if (pTab) pTab->RefreshChildren();
			}
			child = m_treeDatabases->GetNextChild(root, cookie);
		}
	}
	// Refresh the view
	RefreshDbView();
}

void DbViewerPanel::RefreshDbView()
{
	// clear items from tree
	m_treeDatabases->DeleteAllItems();
	// create imageList for icons
	wxImageList *pImageList = new wxImageList(16, 16, true,3);
	pImageList->Add(m_mgr->GetStdIcons()->LoadBitmap(wxT("toolbars/16/standard/file_open"))); // folder icon
	pImageList->Add(m_mgr->GetStdIcons()->LoadBitmap(wxT("db-explorer/16/table")));           // table icon
	pImageList->Add(m_mgr->GetStdIcons()->LoadBitmap(wxT("toolbars/16/search/find")));        // view icon
	pImageList->Add(m_mgr->GetStdIcons()->LoadBitmap(wxT("db-explorer/16/database")));        // database
	pImageList->Add(m_mgr->GetStdIcons()->LoadBitmap(wxT("db-explorer/16/column")));          // column

	m_treeDatabases->AssignImageList(pImageList);

	wxTreeItemId totalRootID = m_treeDatabases->AddRoot(wxString::Format(wxT("Databases")),-1);

	// ---------------- load connections ----------------------------
	SerializableList::compatibility_iterator connectionNode = m_pConnections->GetFirstChildNode();
	while(connectionNode) {
		DbConnection* pDbCon = (DbConnection*) wxDynamicCast(connectionNode->GetData(),DbConnection);
		if (pDbCon) {
			wxTreeItemId rootID = m_treeDatabases->AppendItem(totalRootID,
			                      wxString::Format(wxT("Databases (%s)"),pDbCon->GetServerName().c_str()),
			                      3,
			                      3,
			                      new DbItem(pDbCon));

			// ----------------------- load databases -------------------------------
			SerializableList::compatibility_iterator dbNode = pDbCon->GetFirstChildNode();
			while(dbNode) {
				Database* pDatabase = wxDynamicCast(dbNode->GetData(), Database);
				if (pDatabase) {
					wxTreeItemId dbID = m_treeDatabases->AppendItem(rootID,
					                    pDatabase->GetName(),
					                    3,
					                    3,
					                    new DbItem(pDatabase));
					m_treeDatabases->Expand(rootID);
					wxTreeItemId idFolder = m_treeDatabases->AppendItem(dbID, wxT("Tables"),0,0,NULL);
					//m_treeDatabases->Expand(dbID);

					// ----------------------------- load tables ----------------------------------
					SerializableList::compatibility_iterator tabNode = pDatabase->GetFirstChildNode();
					while(tabNode) {
						DBETable* pTable = wxDynamicCast(tabNode->GetData(), DBETable);
						if (pTable) {
							//wxTreeItemId tabID = m_treeDatabases->AppendItem(idFolder,pTable->getName(),1,-1,new DbItem(NULL,pTable)); //NULL);
							wxTreeItemId tabID = m_treeDatabases->AppendItem(idFolder,
							                     pTable->GetName(),
							                     1,
							                     1,
							                     new DbItem(pTable)); //NULL);

							///////////////////////////////////////////////////////////
							// Add the columns
							///////////////////////////////////////////////////////////
							SerializableList::compatibility_iterator columnNode = pTable->GetFirstChildNode();
							while(columnNode) {
								DBEColumn *col = wxDynamicCast(columnNode->GetData(), DBEColumn);
								if(col) {
									wxTreeItemId colID = m_treeDatabases->AppendItem(tabID,
									                     col->FormatName().c_str(),
									                     4,
									                     4,
									                     new DbItem(col));
								}
								columnNode = columnNode->GetNext();
							}
						}
						tabNode = tabNode->GetNext();
					}
					// ----------------------------------------------------------------------------


					idFolder = m_treeDatabases->AppendItem(dbID, wxT("Views"),0,0,NULL);
					//m_treeDatabases->Expand(dbID);

					// ----------------------------- load views ----------------------------------
					tabNode = pDatabase->GetFirstChildNode();
					while(tabNode) {
						View* pView = wxDynamicCast(tabNode->GetData(),View);
						if (pView) {
							//wxTreeItemId tabID = m_treeDatabases->AppendItem(idFolder,pTable->getName(),1,-1,new DbItem(NULL,pTable)); //NULL);
							wxTreeItemId tabID = m_treeDatabases->AppendItem(idFolder,
							                     pView->GetName(),
							                     2,
							                     2,
							                     new DbItem(pView)); //NULL);
						}
						tabNode = tabNode->GetNext();
					}
					// ----------------------------------------------------------------------------

				}
				dbNode = dbNode->GetNext();
			}
			// -----------------------------------------------------------------------
		}
		connectionNode = connectionNode->GetNext();
	}

}

void DbViewerPanel::OnItemSelectionChange(wxTreeEvent& event)
{
	wxUnusedVar(event);
}

void DbViewerPanel::OnERDClick(wxCommandEvent& event)
{
	AdapterSelectDlg dlg(m_mgr->GetTheApp()->GetTopWindow(), m_pNotebook, m_mgr, m_pConnections);
	dlg.ShowModal();
}

void DbViewerPanel::OnDnDStart(wxTreeEvent& event)
{
	ShapeList lstDnD;
	lstDnD.DeleteContents(true);
	DbItem* item = (DbItem*) m_treeDatabases->GetItemData(event.GetItem());

	ErdPanel* pPanel = wxDynamicCast(m_mgr->GetActivePage(),ErdPanel);

	if (item != NULL) {
		if (pPanel) {
			DBETable* table = wxDynamicCast(item->GetData(),DBETable);
			if (table) {
				table = (DBETable*) table->Clone();
				wxSFShapeBase *pShape = new dndTableShape(table);
				lstDnD.Append(pShape);
				pPanel->GetCanvas()->DoDragDrop(lstDnD);
			}

			View* view = wxDynamicCast(item->GetData(),View);
			if (view) {
				view = (View*) view->Clone();
				wxSFShapeBase *pShape = new dndTableShape(view);
				lstDnD.Append(pShape);
				pPanel->GetCanvas()->DoDragDrop(lstDnD);
			}
		}
	}
}

void DbViewerPanel::OnItemRightClick(wxTreeEvent& event)
{
	m_pEditedDatabase = NULL;
	m_pEditedConnection = NULL;
	m_selectedID = event.GetItem();

	DbItem* item = (DbItem*) m_treeDatabases->GetItemData(m_selectedID);
	wxMenu menu;
	int c = 0;
	if (item) {
		Database* db = wxDynamicCast(item->GetData(),Database);
		if (db) {
			menu.Append(IDR_DBVIEWER_ADD_TABLE,_("Add table"),_("Run SQL command for creating Table"));
			menu.Append(IDR_DBVIEWER_DROP_DATABASE, _("Drop database"), _("Run SQL command for deleting Database"));
			c++;
			c++;
			menu.AppendSeparator();
			menu.Append(IDR_DBVIEWER_ERD_DB, _("Create ERD from DB"),_("Create ERD diagram from database"));
			menu.Append(IDR_DBVIEWER_CLASS_DB, _("Create classes from DB"), _("Create c++ classes for selected database"));
			c++;
			c++;
			menu.AppendSeparator();
			menu.Append(IDR_DBVIEWER_IMPORT_DATABASE, _("Import database from file"), _("Run SQL commands stored in *.sql file"));
			menu.Append(IDR_DBVIEWER_EXPORT_DATABASE, _("Export database to file"), _("Export database CREATE SQL statements into *.sql file"));
			c++;
			c++;

			menu.AppendSeparator();
			menu.Append(IDR_DBVIEWER_DUMP_DATABASE, _("Dump data to file"), _("Dump data from database into .sql file"));
			c++;

			m_pEditedDatabase = db;
		}
		DBETable* tab = wxDynamicCast(item->GetData(), DBETable);
		if (tab) {
			menu.Append(IDR_DBVIEWER_DROP_TABLE,_("Drop table"),_("Run SQL command for deleting Table"));
			c++;
			menu.AppendSeparator();
			menu.Append(IDR_DBVIEWER_ERD_TABLE, _("Create ERD from Table"),_("Create ERD diagram from table"));
			menu.Append(IDR_DBVIEWER_CLASS_TABLE, _("Create classes from Table"), _("Create c++ classes for selected table"));
			c++;
			c++;
		}
	}

	if ( c > 0 ) {
		menu.Connect(wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&DbViewerPanel::OnPopupClick, NULL, this);
		PopupMenu(&menu);
	}


}
void DbViewerPanel::OnToolCloseClick(wxCommandEvent& event)
{
	// Close the active connection (there is only one)
	// getting selected item data
	wxTreeItemIdValue cookie;
	DbItem* data = (DbItem*) m_treeDatabases->GetItemData(m_treeDatabases->GetFirstChild(m_treeDatabases->GetRootItem(), cookie));
	if (data) {
		DbConnection* pCon = wxDynamicCast(data->GetData(), DbConnection);
		if (pCon) {
			wxMessageDialog dlg(this,_("Close connection?"),_("Close"),wxYES_NO);
			if (dlg.ShowModal() == wxID_YES) {
				m_pConnections->GetChildrenList().DeleteContents(true);
				m_pConnections->GetChildrenList().DeleteObject(pCon);
				RefreshDbView();
			}
		}
	}

	// loop over the editor open pages and close all DbExplorer related
	for(size_t i=0; i<m_pagesAdded.Count(); i++) {
		m_mgr->ClosePage(m_pagesAdded.Item(i));
	}

	m_pagesAdded.Clear();
}

void DbViewerPanel::OnToolCloseUI(wxUpdateUIEvent& event)
{
	event.Enable(m_pConnections->HasChildren());
}

void DbViewerPanel::OnPopupClick(wxCommandEvent& evt)
{
	if( !m_selectedID.IsOk() ) return;

	try {
		if (evt.GetId() == IDR_DBVIEWER_ADD_DATABASE) {
			if (m_pEditedConnection) {
				//TODO:LANG:
				wxString dbName = wxGetTextFromUser(_("Database name"), _("Add database"));
				if (!dbName.IsEmpty()) {
					DatabaseLayer* pDbLayer = m_pEditedConnection->GetDbAdapter()->GetDatabaseLayer(wxT(""));
					wxString sql = m_pEditedConnection->GetDbAdapter()->GetCreateDatabaseSql(dbName);
					if (!sql.empty()) {

						pDbLayer->RunQuery(sql);
						pDbLayer->Close();
						delete pDbLayer;
						//TODO:LANG:
						wxMessageBox(_("Database created successfully"));

						RefreshDbView();
					} else {
						//TODO:LANG:
						wxMessageDialog dlg(this, _("Can't create new db in this database engine!"), _("Error"),wxOK|wxICON_ERROR);
						dlg.ShowModal();
					}
				}
			}
		} else if (evt.GetId() == IDR_DBVIEWER_DROP_DATABASE) {
			DbItem* data = (DbItem*) m_treeDatabases->GetItemData(m_selectedID);
			if (data) {
				Database* pDb = (Database*) wxDynamicCast(data->GetData(),Database);
				if (pDb) {
					wxString dropSQL = pDb->GetDbAdapter()->GetDropDatabaseSql(pDb);
					if (!dropSQL.IsEmpty()) {
						//TODO:LANG:
						wxMessageDialog dlg(this, wxString::Format(_("Remove database '%s'?"),pDb->GetName().c_str()),_("Drop database"),wxYES_NO);
						if (dlg.ShowModal() == wxID_YES) {
							DatabaseLayer* pDbLayer = pDb->GetDbAdapter()->GetDatabaseLayer(wxT(""));
							pDbLayer->RunQuery(dropSQL);
							pDbLayer->Close();
							delete pDbLayer;
							//TODO:LANG:
							wxMessageBox(_("Database dropped successfully"));
							RefreshDbView();
						}
					}
				}
			}
		} else if (evt.GetId() == IDR_DBVIEWER_ERD_TABLE) {
			DbItem* data = (DbItem*) m_treeDatabases->GetItemData(m_selectedID);
			if (data) {
				DBETable* pTab = (DBETable*) wxDynamicCast(data->GetData(),DBETable);
				if (pTab) {
					wxString pagename;
					pagename = CreatePanelName(pTab, DbViewerPanel::Erd);
					ErdPanel *erdpanel = new ErdPanel(m_pNotebook,pTab->GetDbAdapter()->Clone(),m_pConnections, (DBETable*) pTab->Clone() );
					m_mgr->AddEditorPage(erdpanel, pagename);
					m_pagesAdded.Add(pagename);
					erdpanel->SetFocus();
				}
			}
		} else if (evt.GetId() == IDR_DBVIEWER_ERD_DB) {
			DbItem* data = (DbItem*) m_treeDatabases->GetItemData(m_selectedID);
			if (data) {
				Database* pDb = (Database*) wxDynamicCast(data->GetData(),Database);
				if (pDb) {
					wxString pagename;
					pagename = CreatePanelName(pDb, DbViewerPanel::Erd);
					ErdPanel *erdpanel = new ErdPanel(m_pNotebook,pDb->GetDbAdapter()->Clone(),m_pConnections, (Database*) pDb->Clone() );
					m_mgr->AddEditorPage(erdpanel, pagename);
					m_pagesAdded.Add(pagename);
					erdpanel->SetFocus();
				}
			}
		} else if (evt.GetId() == IDR_DBVIEWER_CLASS_DB) {
			DbItem* data = (DbItem*) m_treeDatabases->GetItemData(m_selectedID);
			if (data) {
				Database* pDb = (Database*) wxDynamicCast(data->GetData(),Database);
				if (pDb) {
					pDb = (Database*) pDb->Clone();
					ClassGenerateDialog dlg(m_mgr->GetTheApp()->GetTopWindow(), pDb->GetDbAdapter(), (Database*) pDb->Clone(),m_mgr);
					dlg.ShowModal();
				}
			}
		} else if (evt.GetId() == IDR_DBVIEWER_CLASS_TABLE) {
			DbItem* data = (DbItem*) m_treeDatabases->GetItemData(m_selectedID);
			if (data) {
				DBETable* pTab = (DBETable*) wxDynamicCast(data->GetData(),DBETable);
				if (pTab) {
					ClassGenerateDialog dlg(GetParent(), pTab->GetDbAdapter(), (DBETable*) pTab->Clone(), m_mgr);
					dlg.ShowModal();
				}
			}
		} else if (evt.GetId() ==  IDR_DBVIEWER_DROP_TABLE) {
			DbItem* data = (DbItem*) m_treeDatabases->GetItemData(m_selectedID);
			if (data) {
				DBETable* pTab = (DBETable*) wxDynamicCast(data->GetData(),DBETable);
				if (pTab) {
					//TODO:LANG:
					wxMessageDialog dlg(this, wxString::Format(_("Remove table '%s'?"),pTab->GetName().c_str()),_("Drop table"),wxYES_NO);
					if (dlg.ShowModal() == wxID_YES) {
						DatabaseLayer* pDbLayer = pTab->GetDbAdapter()->GetDatabaseLayer(pTab->GetParentName());
						pDbLayer->RunQuery(pTab->GetDbAdapter()->GetDropTableSql(pTab));
						pDbLayer->Close();
						delete pDbLayer;
						//TODO:LANG:
						wxMessageBox(_("Table dropped successfully"));
						RefreshDbView();
					}
				}
			}
		} else if (evt.GetId() == IDR_DBVIEWER_IMPORT_DATABASE) {
			DbItem* data = (DbItem*) m_treeDatabases->GetItemData(m_selectedID);
			if (data) {
				Database* pDb = (Database*) wxDynamicCast(data->GetData(),Database);
				if (pDb) {
					//TODO:LANG:
					wxFileDialog dlg(this, _("Import database from SQL file ..."), wxGetCwd(), wxT(""), wxT("SQL Files (*.sql)|*.sql"), wxFD_OPEN | wxFD_FILE_MUST_EXIST);
					if(dlg.ShowModal() == wxID_OK) {
						ImportDb(dlg.GetPath(), pDb);
					}
				}
			}
			RefreshDbView();
		} else if (evt.GetId() == IDR_DBVIEWER_DUMP_DATABASE) {
			DbItem* data = (DbItem*) m_treeDatabases->GetItemData(m_selectedID);
			if (data) {
				Database* pDb = (Database*) wxDynamicCast(data->GetData(),Database);
				if (pDb) {
					//TODO:LANG:
					wxFileDialog dlg(this, _("Dump data into file ..."),wxT(""), pDb->GetName() + wxT(".sql"),wxT("SQL files (*.sql)|*.sql"), wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
					if( dlg.ShowModal() == wxID_OK ) {
						DumpClass* dump = new DumpClass(pDb->GetDbAdapter(),pDb, dlg.GetPath());
						dump->DumpData();
						wxMessageBox(_("Data was saved to ") + dlg.GetPath() );
					}
				}

			}
		} else if (evt.GetId() == IDR_DBVIEWER_EXPORT_DATABASE) {
			DbItem* data = (DbItem*) m_treeDatabases->GetItemData(m_selectedID);
			if (data) {
				Database* pDb = (Database*) wxDynamicCast(data->GetData(),Database);
				if (pDb) {
					wxFileDialog dlg(this, _("Export database..."), wxGetCwd(), wxT(""), wxT("SQL Files (*.sql)|*.sql"), wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
					if(dlg.ShowModal() == wxID_OK) {
						// CreateStructure
						wxString retStr = wxT("-- SQL script created by wxDbExplorer\n\n ");
						SerializableList::compatibility_iterator tabNode = pDb->GetFirstChildNode();
						while(tabNode) {
							DBETable* tab = wxDynamicCast(tabNode->GetData(),DBETable);
							if (tab) {
								retStr.append(pDb->GetDbAdapter()->GetCreateTableSql(tab,true));
							}
							tabNode = tabNode->GetNext();
						}

						tabNode = pDb->GetFirstChildNode();
						while(tabNode) {
							View* view = wxDynamicCast(tabNode->GetData(),View);
							if (view) {
								retStr.append(pDb->GetDbAdapter()->GetCreateViewSql(view,true));
							}
							tabNode = tabNode->GetNext();
						}

						tabNode = pDb->GetFirstChildNode();
						while(tabNode) {
							DBETable* tab = wxDynamicCast(tabNode->GetData(),DBETable);
							if (tab) {
								retStr.append(pDb->GetDbAdapter()->GetAlterTableConstraintSql(tab));
							}
							tabNode = tabNode->GetNext();
						}

						DumpClass dump(pDb->GetDbAdapter(), pDb, dlg.GetPath());
						dump.DumpData();


						wxTextFile file(dlg.GetPath());
						if (!file.Exists()) file.Create();
						file.Open();
						if (file.IsOpened()) {
							file.InsertLine(retStr,0);
							file.Write();
							file.Close();
						}

						wxMessageBox(wxString::Format(_("The database has been exported to '%s'."), dlg.GetPath().GetData()), _("wxDbExplorer"));
					}

				}
			}
		} else {
			wxMessageBox(_("Sorry, requested feature isn't implemented yet. "),_("Sorry"));
		}
	} catch (DatabaseLayerException& e) {
		wxString errorMessage = wxString::Format(_("Error (%d): %s"), e.GetErrorCode(), e.GetErrorMessage().c_str());
		wxMessageDialog dlg(this,errorMessage,_("DB Error"),wxOK | wxCENTER | wxICON_ERROR);
		dlg.ShowModal();
	} catch( ... ) {
		wxMessageDialog dlg(this,_("Unknown error."),_("DB Error"),wxOK | wxCENTER | wxICON_ERROR);
		dlg.ShowModal();
	}
}

bool DbViewerPanel::ImportDb(const wxString& sqlFile, Database* pDb)
{
	DatabaseLayer* pDbLayer = NULL;
	LogDialog dialog( this );//TODO:Doresit parenta
	dialog.Show();

	try {
		wxFileInputStream input(sqlFile);
		wxTextInputStream text( input );
		text.SetStringSeparators(wxT(";"));
		wxString command = wxT("");


		pDbLayer = pDb->GetDbAdapter()->GetDatabaseLayer(pDb->GetName());
		pDbLayer->BeginTransaction();

		wxString useSql = pDb->GetDbAdapter()->GetUseDb(pDb->GetName());
		if (!useSql.IsEmpty()) pDbLayer->RunQuery(wxString::Format(wxT("USE %s"), pDb->GetName().c_str()));

		while (!input.Eof()) {
			wxString line = text.ReadLine();
			//dialog.AppendText(line);
			int index = line.Find(wxT("--"));
			if (index != wxNOT_FOUND) line = line.Mid(0,index);
			command.append(line);
			if (line.Find(wxT(";")) != wxNOT_FOUND) {
				dialog.AppendSeparator();
				dialog.AppendComment(wxT("Run SQL command:"));
				dialog.AppendText(command);
				pDbLayer->RunQuery(command);
				dialog.AppendComment(_("Successful!"));
				command.clear();
			}
		}
		pDbLayer->Commit();
		pDbLayer->Close();
	} catch (DatabaseLayerException& e) {
		if (pDbLayer) {
			pDbLayer->RollBack();
			pDbLayer->Close();
		}
		wxString errorMessage = wxString::Format(_("Error (%d): %s"), e.GetErrorCode(), e.GetErrorMessage().c_str());

		dialog.AppendComment(_("Fail!"));
		dialog.AppendComment(errorMessage);
		wxMessageDialog dlg(this,errorMessage,_("DB Error"),wxOK | wxCENTER | wxICON_ERROR);
		dlg.ShowModal();
	} catch( ... ) {
		if (pDbLayer) {
			pDbLayer->RollBack();
			pDbLayer->Close();
		}
		wxMessageDialog dlg(this,_("Unknown error."),_("DB Error"),wxOK | wxCENTER | wxICON_ERROR);
		dlg.ShowModal();
	}
	dialog.EnableClose(true);
	dialog.ShowModal();


	return false;
}

void DbViewerPanel::OnPageChange(NotebookEvent& event)
{
	ErdPanel* pPanel = wxDynamicCast(m_mgr->GetActivePage(),ErdPanel);
	if (pPanel)	m_pThumbnail->SetCanvas(pPanel->GetCanvas());

	event.Skip();
}

void DbViewerPanel::OnPageClose(NotebookEvent& event)
{
	ErdPanel* pPanel = wxDynamicCast(m_mgr->GetPage(event.GetSelection()),ErdPanel);
	if (pPanel)	{
		m_pThumbnail->SetCanvas(NULL);
	}
	event.Skip();
}

wxString DbViewerPanel::CreatePanelName(Database* d, PanelType type)
{
	if( type == DbViewerPanel::Sql ) {
		return wxT("SQL [") + d->GetName() + wxT("]");
	} else
		return wxT("ERD [") + d->GetName() + wxT("]");
}

wxString DbViewerPanel::CreatePanelName(DBETable* t, PanelType type)
{
	if( type == DbViewerPanel::Sql ) {
		return wxT("SQL [") + t->GetParentName() + wxT(":") + t->GetName() + wxT("]");
	} else
		return wxT("ERD [") + t->GetParentName() + wxT(":") + t->GetName() +  wxT("]");
}

wxString DbViewerPanel::CreatePanelName(View* v, PanelType type)
{
	if( type == DbViewerPanel::Sql ) {
		return wxT("SQL [") + v->GetParentName() + wxT(":") + v->GetName() + wxT("]");
	} else
		return wxT("ERD [") + v->GetParentName() + wxT(":") + v->GetName() +  wxT("]");
}
void DbViewerPanel::InitStyledTextCtrl(wxScintilla *sci)
{
	sci->SetLexer( wxSCI_LEX_SQL );
	sci->SetKeyWords(0, wxT("select insert into delete update from drop create alter where values order by desc asc show table column tables columns limit as in exists view join left inner set") );
	wxFont font = wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT);
	font.SetFamily(wxTELETYPE);

	for(int i=0; i<wxSCI_STYLE_MAX; i++)
		sci->StyleSetFont( wxSCI_STYLE_MAX, font);

	sci->StyleSetBold( wxSCI_C_WORD, true );
	sci->StyleSetForeground( wxSCI_C_WORD, *wxBLUE );
	sci->StyleSetForeground( wxSCI_C_STRING, *wxRED );
	sci->StyleSetForeground( wxSCI_C_STRINGEOL, *wxRED );
	sci->StyleSetForeground( wxSCI_C_PREPROCESSOR, wxColour( 49, 106, 197 ) );
	sci->StyleSetForeground( wxSCI_C_COMMENT, wxColour( 0, 128, 0 ) );
	sci->StyleSetForeground( wxSCI_C_COMMENTLINE, wxColour( 0, 128, 0 ) );
	sci->StyleSetForeground( wxSCI_C_COMMENTDOC, wxColour( 0, 128, 0 ) );
	sci->StyleSetForeground( wxSCI_C_COMMENTLINEDOC, wxColour( 0, 128, 0 ) );
	sci->StyleSetForeground( wxSCI_C_NUMBER, *wxGREEN );

	sci->SetSelBackground( true, wxSystemSettings::GetColour( wxSYS_COLOUR_HIGHLIGHT ) );
	sci->SetSelForeground( true, wxSystemSettings::GetColour( wxSYS_COLOUR_HIGHLIGHTTEXT ) );

	sci->SetCaretWidth( 2 );

	sci->SetTabIndents(true);
	sci->SetBackSpaceUnIndents(true);
	sci->SetUseTabs( false );
	sci->SetTabWidth( 4 );
	sci->SetIndent( 4 );

	// markers
	sci->MarkerDefine(wxSCI_MARKNUM_FOLDER,        wxSCI_MARK_BOXPLUS);
	sci->MarkerSetBackground( wxSCI_MARKNUM_FOLDER, wxColour( wxT("DARK GREY") ) );
	sci->MarkerSetForeground( wxSCI_MARKNUM_FOLDER, wxColour( wxT("WHITE") ) );

	sci->MarkerDefine(wxSCI_MARKNUM_FOLDEROPEN,    wxSCI_MARK_BOXMINUS);
	sci->MarkerSetBackground( wxSCI_MARKNUM_FOLDEROPEN, wxColour( wxT("DARK GREY") ) );
	sci->MarkerSetForeground( wxSCI_MARKNUM_FOLDEROPEN, wxColour( wxT("WHITE") ) );

	sci->MarkerDefine(wxSCI_MARKNUM_FOLDERSUB,     wxSCI_MARK_VLINE);
	sci->MarkerSetBackground( wxSCI_MARKNUM_FOLDERSUB, wxColour( wxT("DARK GREY") ) );
	sci->MarkerSetForeground( wxSCI_MARKNUM_FOLDERSUB, wxColour( wxT("WHITE") ) );

	sci->MarkerDefine(wxSCI_MARKNUM_FOLDEREND,     wxSCI_MARK_EMPTY);
	sci->MarkerSetBackground( wxSCI_MARKNUM_FOLDEREND, wxColour( wxT("DARK GREY") ) );
	sci->MarkerSetForeground( wxSCI_MARKNUM_FOLDEREND, wxColour( wxT("WHITE") ) );

	sci->MarkerDefine(wxSCI_MARKNUM_FOLDEROPENMID, wxSCI_MARK_BOXMINUS);
	sci->MarkerSetBackground( wxSCI_MARKNUM_FOLDEROPENMID, wxColour( wxT("DARK GREY") ) );
	sci->MarkerSetForeground( wxSCI_MARKNUM_FOLDEROPENMID, wxColour( wxT("WHITE") ) );

	sci->MarkerDefine(wxSCI_MARKNUM_FOLDERMIDTAIL, wxSCI_MARK_TCORNER);
	sci->MarkerSetBackground( wxSCI_MARKNUM_FOLDERMIDTAIL, wxColour( wxT("DARK GREY") ) );
	sci->MarkerSetForeground( wxSCI_MARKNUM_FOLDERMIDTAIL, wxColour( wxT("WHITE") ) );

	sci->MarkerDefine(wxSCI_MARKNUM_FOLDERTAIL,    wxSCI_MARK_LCORNER);
	sci->MarkerSetBackground( wxSCI_MARKNUM_FOLDERTAIL, wxColour( wxT("DARK GREY") ) );
	sci->MarkerSetForeground( wxSCI_MARKNUM_FOLDERTAIL, wxColour( wxT("WHITE") ) );

	sci->SetFoldFlags( wxSCI_FOLDFLAG_LINEAFTER_CONTRACTED );
}

void DbViewerPanel::OnShowThumbnail(wxCommandEvent& e)
{
	if(e.IsChecked()) {
		GetSizer()->Show(m_pThumbnail);

	} else {
		GetSizer()->Hide(m_pThumbnail);

	}
	GetSizer()->Layout();
}

bool DbViewerPanel::DoSelectPage(const wxString& page)
{
	wxWindow *win = m_mgr->FindPage(page);
	if(win) {
		m_mgr->SelectPage(win);
		win->SetFocus();

		// replace the query and execute it
		SQLCommandPanel *sqlPage = dynamic_cast<SQLCommandPanel*>(win);
		if(sqlPage) {
			sqlPage->SetDefaultSelect();
			sqlPage->ExecuteSql();
		}
		return true;
	}
	return false;
}
