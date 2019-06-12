//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

import Ice
import PromiseKit
import UIKit

class MainController: UIViewController,
    DetailControllerDelegate {
    @IBOutlet var searchTableView: UITableView!
    var communicator: Ice.Communicator!
    var session: SessionAdapter!
    var library: LibraryPrx!
    var books: [BookDescription]
    var query: BookQueryResultPrx?
    var nrows: Int
    var rowsQueried: Int
    var currentIndexPath: IndexPath!

    required init?(coder: NSCoder) {
        books = []
        nrows = 0
        rowsQueried = 0
        super.init(coder: coder)
    }

    override func viewDidLoad() {
        super.viewDidLoad()
        NotificationCenter.default.addObserver(self,
                                               selector: #selector(destroySession),
                                               name: UIApplication.willTerminateNotification,
                                               object: nil)
    }

    func activate(communicator: Ice.Communicator,
                  session: SessionAdapter,
                  library: LibraryPrx?) {
        self.communicator = communicator
        self.session = session
        self.library = library
        query = nil
        nrows = 0
        rowsQueried = 10
        books.removeAll()
    }

    @objc func destroySession() {
        precondition(session != nil)
        session.destroy()
        session = nil

        //
        // Destroy the communicator from another thread since this call blocks.
        //
        DispatchQueue.global().async {
            self.communicator.destroy()
            self.communicator = nil
        }
    }

    @IBAction func logout(sender _: AnyObject) {
        destroySession()
        navigationController?.popViewController(animated: true)
    }

    func removeCurrentBook() {
        let book = books[currentIndexPath.row]

        if let proxy = book.proxy {
            proxy.destroyAsync().done {
                UIApplication.shared.isNetworkActivityIndicatorVisible = false
            }.catch { e in
                if e is Ice.ObjectNotExistException {
                    return
                }
                self.exception(e)
            }
        }
        // Remove the book, and the row from the table.
        books.remove(at: currentIndexPath.row)
        nrows -= 1
        searchTableView.deleteRows(at: [currentIndexPath], with: .fade)
    }

    func bookUpdated(_ book: BookDescription) {
        if let currentIndexPath = self.currentIndexPath {
            books[currentIndexPath.row] = book
        }
        searchTableView.reloadData()
    }

    func bookDeleted() {
        removeCurrentBook()
    }

    override func viewWillAppear(_: Bool) {
        //
        // There was a fatal error and the session was destroyed.
        //
        if session == nil {
            navigationController?.popViewController(animated: true)
        }
        //
        // Redisplay the data.
        //
        searchTableView.reloadData()
    }

    override func prepare(for segue: UIStoryboardSegue, sender _: Any?) {
        if segue.identifier == "ShowDetail", let dc = segue.destination as? DetailController {
            dc.delegate = self
            dc.startEdit(books[currentIndexPath.row])
        } else if segue.identifier == "NewBook", let ac = segue.destination as? AddController {
            ac.delegate = self
            ac.startEdit(book: BookDescription(), library: library)
        }
    }

    func exception(_ error: Error) {
        UIApplication.shared.isNetworkActivityIndicatorVisible = false
        //
        // BUGFIX: In the event of a fatal exception we want to pop back to the login view.
        // However, doing so directly by calling [self.navigationController popToRootViewControllerAnimated:YES];
        // causes the navigation view & the bar to get out of sync. So instead, we pop to the root view
        // in the alert view didDismissWithButtonIndex callback.
        //
        destroySession()

        //
        // open an alert with just an OK button
        //
        let alert = UIAlertController(title: "Error",
                                      message: error.localizedDescription,
                                      preferredStyle: .alert)
        alert.addAction(UIAlertAction(title: "OK", style: .default) { _ in
            self.navigationController?.popViewController(animated: true)
        })
        present(alert, animated: true, completion: nil)
    }
}

extension MainController: UISearchBarDelegate {
    func searchBarCancelButtonClicked(_ searchBar: UISearchBar) {
        searchBar.resignFirstResponder()
        searchBar.showsScopeBar = false
        searchBar.showsCancelButton = false
    }

    func searchBarSearchButtonClicked(_ searchBar: UISearchBar) {
        searchBar.resignFirstResponder()
        searchBar.showsScopeBar = false
        searchBar.showsCancelButton = false

        let search = searchBar.text!
        let searchMode = searchBar.selectedScopeButtonIndex

        // Kill the previous query results.
        query = nil
        nrows = 0
        rowsQueried = 10
        books.removeAll()
        searchTableView.reloadData()

        UIApplication.shared.isNetworkActivityIndicatorVisible = true

        var result: Promise<(first: BookDescriptionSeq, nrows: Swift.Int32, result: BookQueryResultPrx?)>!
        switch searchMode {
        case 0:
            result = library.queryByIsbnAsync(isbn: search, n: 10)
        case 1:
            result = library.queryByAuthorAsync(author: search, n: 10)
        case 2:
            result = library.queryByTitleAsync(title: search, n: 10)
        default:
            fatalError()
        }

        result.done { r in
            UIApplication.shared.isNetworkActivityIndicatorVisible = false
            self.nrows = Int(r.nrows)
            if self.nrows == 0 {
                //
                // open an alert with just an OK button
                //
                let alert = UIAlertController(title: "No results",
                                              message: "The search returned no results",
                                              preferredStyle: .alert)
                alert.addAction(UIAlertAction(title: "OK", style: .default, handler: nil))
                self.present(alert, animated: true, completion: nil)
                return
            }
            self.books.append(contentsOf: r.first)
            self.query = r.result
            self.searchTableView.reloadData()
        }.catch { error in
            self.exception(error)
        }
    }

    func searchBarShouldBeginEditing(_ searchBar: UISearchBar) -> Bool {
        searchBar.showsScopeBar = true
        searchBar.showsCancelButton = true
        return true
    }
}

extension MainController: UITableViewDelegate, UITableViewDataSource {
    func numberOfSectionsInTableView() -> Int {
        return 1
    }

    func tableView(_: UITableView, numberOfRowsInSection _: Int) -> Int {
        return nrows
    }

    func tableView(_: UITableView, cellForRowAt indexPath: IndexPath) -> UITableViewCell {
        let cell = searchTableView.dequeueReusableCell(withIdentifier: "MyIdentifier") ??
            UITableViewCell(style: .default, reuseIdentifier: "MyIdentifier")
        cell.accessoryType = UITableViewCell.AccessoryType.detailDisclosureButton

        if indexPath.row > books.count - 1 {
            //
            // Here we are past the available cached set of data. rowsQueried records
            // how many rows of data we've actually asked for.
            //
            if indexPath.row > rowsQueried - 1 {
                UIApplication.shared.isNetworkActivityIndicatorVisible = true
                precondition(query != nil)

                query!.nextAsync(Int32(10)).done { result in
                    self.books.append(contentsOf: result.returnValue)
                    if result.destroyed {
                        self.query = nil
                    }
                    UIApplication.shared.isNetworkActivityIndicatorVisible = true
                    self.searchTableView.reloadData()
                }.catch { error in
                    self.exception(error)
                }
                rowsQueried += 10
            }
            cell.textLabel!.text = "<loading>"
        } else {
            cell.textLabel!.text = books[indexPath.row].title
        }
        return cell
    }

    func tableView(_: UITableView, willSelectRowAt indexPath: IndexPath) -> IndexPath? {
        // Selecting a <loading> book does nothing.
        if indexPath.row > books.count - 1 {
            return nil
        }

        currentIndexPath = indexPath
        performSegue(withIdentifier: "ShowDetail", sender: self)
        return nil
    }
}
