  Apollonius_graph ag;

  SiteList::iterator itr;
  // add all original sites to the apollonius graph
  for (itr = sites.begin(); itr != sites.end(); ++itr) {
    Site_2 site = *itr;
    ag.insert(site);
  }
  // add all artificial sites to the apollonius graph
  for (itr = artificialSites.begin(); itr != artificialSites.end(); ++itr) {
    Site_2 site = *itr;
    ag.insert(site);
  }
