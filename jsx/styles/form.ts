import styled from 'styled-components';
import Layout from './layout';

export const HorizontalRule = styled.hr`
  border-top: 1.5px solid #DDDDDD;
  padding-top: 15px;
  margin-top: 0;
`;

// Extend FlexBox for ListContainer with specific styles
export const ListContainer = styled(Layout.Column)`
  border: 1px solid #DDD;
  border-radius: 10px;
  min-height: 85px;
  padding: 5px;
  margin-bottom: 15px;
`;

// Extend FlexBox for ListItem with specific layout tweaks
export const ListItem = styled(Layout.Row).attrs({
  justify: 'space-between', // Justify content space-between
  align: 'flex-start' // Align items to flex-start
})`
  width: 100%;
  &:not(:last-child) {
    margin-bottom: 10px;
  }
`;

// Icon with clickable action
export const Icon = styled.span<{className?: string, onClick?: () => void}>`
  color: #DDDDDD;
  margin-left: 10px;
  cursor: pointer;
  font-size: 20px; // Assuming you're using a font icon like FontAwesome or Glyphicons
`;

export const CollapsibleContent = styled.div<{
  collapsed: boolean
  children: React.ReactNode
}>`
  transition: max-height 0.3s ease, opacity 0.3s ease;
  max-height: ${(props) => (props.collapsed ? '0' : '1000px')}; // Adjust as necessary for your content
  opacity: ${(props) => (props.collapsed ? '0' : '1')};
  overflow: hidden;
`;

// display: ${(props) => (props.collapsed ? 'none' : 'inline-block')};
export const CollapseButton = styled.span<{
  collapsed: boolean
  onClick: () => void;
}>`
  cursor: pointer;
  font-size: 15px;
  position: relative;
  right: 40px;
  &::before {
    content: ${props => props.collapsed ? "'\\e114'" : "'\\e080'"}; // Unicode for glyphicon icons (example)
    font-family: 'Glyphicons Halflings'; // Assuming glyphicon is in use
  }
`;

// const CollapseButton: React.FC<{
//   collapsed: boolean,
//   onClick: () => void
// }> = ({
//   collapsed,
//   onClick
// }) => (
//   <span
//     onClick={onClick}
//     className={'glyphicon '+ collapsed ? 'glyphicon-chevron-down' : 'glyphicon-chevron-up'}
//     style={{
//       cursor: 'pointer',
//       fontSize: 15,
//       position: 'relative',
//       right: 40,
//     }}
//   />
// );
// 
// const RemoveItemButton: React.FC<{onClick: () => void}> = ({ onClick }) => (
//   <span
//     onClick={onClick}
//     className='glyphicon glyphicon-remove'
//     style={{
//       color: '#808080',
//       marginLeft: 10,
//       cursor: 'pointer',
//       fontSize: 15,
//     }}
//   />
// );

const Form = {
  HorizontalRule,
  ListContainer,
  ListItem,
  Icon,
  CollapsibleContent,
  CollapseButton,
};

export default Form;
