import styled from 'styled-components';

type CSSAlignSelf = 'auto' | 'start' | 'end' | 'center' | 'baseline' | 'stretch';
type CSSJustifySelf = 'auto' | 'start' | 'end' | 'center' | 'stretch';
type CSSAlignItems = 'stretch' | 'center' | 'start' | 'end' | 'baseline';
type CSSJustifyContent = 'start' | 'end' | 'center' | 'space-between' | 'space-around' | 'space-evenly';

const Grid = styled.div<{
  columns?: string;
  rows?: string;
  gap?: string;
  columnGap?: string;
  rowGap?: string;
  align?: CSSAlignItems;
  justify?: CSSJustifyContent;
}>`
  display: grid;
  grid-template-columns: ${({ columns }) => columns || '1fr'};
  grid-template-rows: ${({ rows }) => rows || 'auto'};
  gap: ${({ gap }) => gap || '0'};
  column-gap: ${({ columnGap }) => columnGap || '0'};
  row-gap: ${({ rowGap }) => rowGap || '0'};
  align-items: ${({ align }) => align || 'center'};
  justify-content: ${({ justify }) => justify || 'center'};
`;

const Item = styled.div<{
  column?: string;
  row?: string;
  align?: CSSAlignSelf;
  justify?: CSSJustifySelf;
}>`
  grid-column: ${({ column }) => column || 'auto'};
  grid-row: ${({ row }) => row || 'auto'};
  align-self: ${({ align }) => align || 'auto'};
  justify-self: ${({ justify }) => justify || 'auto'};
`;

// GridBox Styled Component
const Field = styled(Grid)<{ hasLabel: boolean }>`
  grid-template-columns: ${({ hasLabel }) => hasLabel ? '2fr 3fr' : '1fr'}; /* Adjust based on label presence */
  gap: 10px; /* Space between label and children */

  @media (max-width: 600px) { /* Adjust breakpoint as needed */
    grid-template-columns: 1fr; /* Stack elements vertically on small screens */
  }
`;

const Flex = styled.div<{
  direction?: 'row' | 'column';
  align?: 'stretch' | 'center' | 'flex-start' | 'flex-end' | 'baseline';
  justify?: 'flex-start' | 'center' | 'flex-end' | 'space-between' | 'space-around' | 'space-evenly';
  wrap?: 'nowrap' | 'wrap' | 'wrap-reverse';
  width?: string;
  height?: string;
}>`
  display: flex;
  flex-direction: ${({ direction }) => direction || 'row'};
  align-items: ${({ align }) => align || 'center'};
  justify-content: ${({ justify }) => justify || 'center'};
  flex-wrap: ${({ wrap }) => wrap || 'nowrap'};
  width: ${({ width }) => width || '100%'};
  height: ${({ height }) => height || 'auto'};
`;

const Row = styled(Flex)<{
  flex?: number
}>`
  flex-direction: row;
  flex: ${({flex}) => flex || '1'};
`;

const Column = styled(Flex)<{
  flex?: number
}>`
  flex-direction: column;
  flex: ${({flex}) => flex || '1'};
`;

// const Row = styled(GridBox)`
//   grid-auto-flow: row; // Makes it behave like a flex row but with grid capabilities
// `;
// 
// const Column = styled.div<{ cols?: string }>`
//   grid-column: ${({ cols }) => cols || 'auto'};
//   grid-auto-flow: column; // Makes it behave like a flex column but with grid capabilities
// `;

const Container = styled.div`
  width: 100%;
  max-width: 1200px;
  margin: 0 auto;
  padding: 0 15px;
`;

const Inline = styled.div`
  display: inline-block;

  // Applying styles to direct children
  > * {
    margin: 0 5px;
  }
`;

const ActionTitle = styled.div`
  font-size: 16px;
  display: inline;

  // Applying styles to direct children
  > * {
    margin: 0 5px;
  }
`;

// const Input = styled.input`
//   width: 50px;
//   display: inline;
//   margin-right: 10px;
// `;


// export const Input = styled.input.attrs(props => ({
//   type: 'number',
//   min: 1,
//   max: 50
// }))`
//   width: 50px;
//   display: inline;
//   margin-right: 10px;
// `;

export const Layout = {
  Grid,
  Item,
  Field,
  Row,
  Column,
  Inline,
  ActionTitle,
};
