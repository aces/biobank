import React from 'react';
import { Link as RouterLink } from 'react-router-dom';

type LinkProps = {
  to?: string; // URL for <Link> component
  href?: string; // URL for <a> tag
  condition?: boolean; // Renamed from 'if' to 'condition'
  children: React.ReactNode;
};

const Link: React.FC<LinkProps> = ({ to, href, condition, children }) => {
  if (!condition) { // Updated to use 'condition'
    return <>{children}</>;
  }

  if (to) {
    return <RouterLink to={to}>{children}</RouterLink>;
  } else if (href) {
    return <a href={href}>{children}</a>;
  } else {
    return <>{children}</>; // Render just the children if no link is provided
  }
};

export default Link;

