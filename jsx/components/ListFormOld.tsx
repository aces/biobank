// import React, { useState, useEffect } from 'react';
// import { clone, isEmpty } from '../utils';
// 
// interface ListFormProps<T, H> {
//   list: Map<number, [T, H]>,
//   errors: Record<string, any>;
//   setList: (list: Map<number, [T, H]>) => void;
//   listItem: () => [T, H];
// }
// 
// const ListForm = <T, H>({
//   children,
//   list,
//   errors,
//   setList,
//   listItem
// }: ListFormProps<T, H>): ReactElement => {
//   const [count, setCount] = useState<number>(0);
//   const [multiplier, setMultiplier] = useState<number>(1);
//   const [collapsed, setCollapsed] = useState<Map<number, boolean>>(new Map());
//   
//   useEffect(() => {
//     if(list.length === 0) addListItem();
//   }, []); // Runs once on mount
// 
//   useEffect(() => {
//     list.forEach((key) => {
//       if (!isEmpty(errors[key]) && collapsed[key]) {
//         toggleCollapse(key);
//       }
//     });
//   }, [list, errors, collapsed]); // Dependencies array, runs on changes to these variables
// 
//   /**
//    * Toggle whether a key is collapsed
//    *
//    * @param {string} key - the key to toggle
//    */
//   const toggleCollapse = useCalledback((key: number) => {
//     const newCollapsed = {...collapsed};
//     newCollapsed[key] = !newCollapsed[key];
//     setCollapsed(newCollapsed);
//   }, []);
// 
//   /**
//    * Set the value of a list item
//    *
//    * @param {string} name - the item name
//    * @param {string} value - the item value
//    * @param {string} key - the key with the item
//    */
//   const setListItem = (name: string, value: string, key: number) => {
//     // 1 represents the handler here
//     list[key][1].set(name, value);
//   }
// 
//   /**
//    * Add an empty list item
//    */
//   const addListItem = () => {
//     const newCount = count + 1;
//     const newCollapsed = {...collapsed, [newCount]: false};
//     const newList = {...list, [newCount]: listItem()};
// 
//     setCount(newCount);
//     setCollapsed(newCollapsed);
//     setList(newList);
//   };
// 
//   /**
//    * Copy an item in a list
//    *
//    * @param {string} key - the key to copy
//    */
//   const copyListItem = (key: number) => {
//     let newCount = count;
//     const newList = [...list];
//     const newCollapsed = {...collapsed};
// 
//     for (let i=1; i<=multiplier; i++) {
//        newCount++;
//        // Use the data from the item you're copying as the initial state for the new item
//        const originalItemData = newList[key][0];
// 
//        // Assuming `listItem` is your hook that initializes a new item and its handler
//        // You might need to adjust how you pass the initial data to `listItem` based on its implementation
//        const newItemTuple = listItem(originalItemData);
// 
// 
//        // Exempt certain elements from being copied
//        if (newList[newCount].container && newList[newCount].container.barcode) {
//          newItemTuple[1].remove('barcode');
//        }
// 
//        newList[newCount](newItemTuple);
//        newCollapsed[newCount] = true;
//     }
// 
//     setCount(newCount);
//     setCollapsed(newCollapsed);
//     setList(newList);
//   }
// 
//   /**
//    * Remove a list item from the list
//    *
//    * @param {string} key - the key to remove
//    */
//   const removeListItem = useCallback((key: number) => {
//     const newList = clone(list);
//     delete newList[key];
//     setList(newList);
//   });
// 
//   return list.map(([key, item], i, listArray) => {
//     const listLength = list.length;
//     const handleRemoveItem = listLength > 1 ?
//         () => removeListItem(key) : null;
//     const handleCopyItem = () => copyListItem(key);
//     const handleCollapse = () => toggleCollapse(key);
// 
//     return subForms.map((subForm) => React.Children.map(children, (child) => {
//       const form = React.cloneElement(child, {
//         key: key,
//         itemKey: key,
//         id: (i + 1),
//         collapsed: collapsed[key],
//         handleCollapse: handleCollapse,
//         item: (item || {}),
//         removeItem: handleRemoveItem,
//         setListItem: setListItem,
//         errors: (errors[key] || {}),
//       });
// 
//       const renderAddButtons = () => {
//         if (i+1 == listLength) {
//           return (
//             <div className='row'>
//               <div className='col-xs-12'>
//                 <div className='col-xs-3'/>
//                 <div className='col-xs-4 action'>
//                   <div>
//                     <span className='action'>
//                       <div
//                         className='action-button add'
//                         onClick={addListItem}
//                       >
//                         +
//                       </div>
//                     </span>
//                     <span className='action-title'>
//                       New Entry
//                     </span>
//                   </div>
//                 </div>
//                 <div className='col-xs-5 action'>
//                   <div>
//                     <span className='action'>
//                       <div
//                        className='action-button add'
//                        onClick={handleCopyItem}
//                       >
//                         <span className='glyphicon glyphicon-duplicate'/>
//                       </div>
//                     </span>
//                     <span className='action-title'>
//                       <input
//                         className='form-control input-sm'
//                         type='number'
//                         min='1'
//                         max='50'
//                         style={{width: 50, display: 'inline'}}
//                         value={multiplier}
//                         onChange={(e) => setMultiplier(Number(e.target.value))}
//                       />
//                       Copies
//                     </span>
//                   </div>
//                 </div>
//               </div>
//             </div>
//           );
//         }
//       };
// 
//       return (
//         <div>
//           {form}
//           {renderAddButtons()}
//         </div>
//       );
//     });
//   });
// }
// 
